#include <Carcassonne/AI/DeepRLPlayer.h>
// #define SPDLOG_FMT_EXTERNAL
#include <spdlog/spdlog.h>
#include <iostream>

namespace carcassonne::ai {

DeepRLPlayer::DeepRLPlayer(
   IGame &game,
   Player player,
   std::mt19937 &generator,
   std::unique_ptr<carcassonne::ai::rl::thread_pool> &workers_pool,
   unsigned trees_count = 1 )
    : m_player_count(game.player_count())
    , m_player(player)
    , m_move_readyness(std::make_shared<rl::DataWrapper<rl::MoveReadyness>>())
    , m_ready_to_move(std::make_shared<std::condition_variable>())
    , m_move_found(std::make_shared<std::condition_variable>())
    , m_generator(generator)
    , m_ctx_ptr(std::make_unique<rl::Context>(  game,
                                                player,
                                                m_last_moves,
                                                workers_pool,
                                                m_move_readyness,
                                                m_ready_to_move,
                                                m_move_found) )
    , m_trees_count(trees_count)
    , m_clients_pool(std::make_unique<rl::client_threads>(m_trees_count, m_ctx_ptr, m_player) )
{
   spdlog::info("deep rl: initialising agent");
   std::this_thread::sleep_for(std::chrono::seconds(5));
   std::cout << "cpus=" << std::thread::hardware_concurrency() << std::endl;
   game.on_next_move([this](IGame &game, Player player, FullMove last_move) {
      m_last_moves[static_cast<mb::size>(last_player(player, m_player_count))] = last_move;
      if (player != m_player)
         return;
      make_move(game);
   });
   spdlog::info("deep rl: initialised agent");
}

bool prepare_tree(std::unique_ptr<rl::Context> &ctx_ptr, Player m_player) {
   ctx_ptr->lck.lock();
   std::unique_ptr<Tree>& tree = ctx_ptr->trees[std::this_thread::get_id()];
   ctx_ptr->lck.unlock();
   if (nullptr == tree) return 1;
   NodePtr node = tree->root();
   auto player = m_player;
   do {
      node = tree->find_node_by_move(node, ctx_ptr->last_moves[static_cast<mb::size>(player)]);
      if (node == nullptr)
         break;
      player = next_player(player, ctx_ptr->game.player_count());
   } while (player != m_player);

   if (node == nullptr) {
      spdlog::debug("deep rl: building MCTS tree from scratch");
      tree->reset(ctx_ptr->game, m_player);
      return 0;
   }
   spdlog::debug("deep rl: reusing existing MCTS tree node");
   tree->change_root(node);
   return 0;
}

void rl::client_threads::client_work(unsigned cpu_id) {
   spdlog::debug("client_work: client run on cpu '{}'", cpu_id);
   std::this_thread::sleep_for(std::chrono::seconds(1));
   m_ctx_ptr->lck.lock();
   if (!m_ctx_ptr->leading_tread_assigned) {
      m_ctx_ptr->leading_tread_id = std::this_thread::get_id();
   }
   spdlog::debug("thread {} pushes tree", thread_name());
   m_ctx_ptr->trees.emplace(std::this_thread::get_id(), std::make_unique<Tree>(m_ctx_ptr->game, m_ctx_ptr->player));
   spdlog::debug("thread {} pushed tree", thread_name());
   m_ctx_ptr->lck.unlock();
   FullMove best_move;
   Node* node_with_best_move = nullptr;
   spdlog::debug("client_work: client '{}' started", cpu_id);
   while (true) {
      spdlog::info("client_work: client '{}' waiting...", cpu_id);
      std::unique_lock<std::mutex> lck(m_ctx_ptr->move_readyness->m_mutex);
      m_ctx_ptr->ready_to_move->wait(lck, [this] {
         spdlog::info("client_work: dataReady={}",
                      (m_ctx_ptr->move_readyness->m_data.dataReady ? "true" : "false"));
         return m_ctx_ptr->move_readyness->m_data.dataReady || m_ctx_ptr->move_readyness->terminate;
      });
      m_ctx_ptr->move_readyness->m_data.dataReady = false;
      if (m_ctx_ptr->move_readyness->terminate) {
         spdlog::info("client_work: terminate");
         break;
      }
      spdlog::info("client_work: client '{}' Running !!!");
      if (prepare_tree(m_ctx_ptr, m_player)) {
         spdlog::debug("nullptr == tree");
         return;
      }
      bool move_is_illegal = true;
      do {
         rl::run_mcts(m_ctx_ptr, 0, 1600);
         node_with_best_move = rl::choose_move(m_ctx_ptr, m_ctx_ptr->game.move_index());
         best_move = node_with_best_move->move();
         m_ctx_ptr->last_moves[static_cast<int>(m_ctx_ptr->player)] = best_move;
         if (best_move.ignored_figure) {
            if (m_ctx_ptr->game.board().can_place_at(best_move.x, best_move.y, m_ctx_ptr->game.tile_set()[m_ctx_ptr->game.move_index()], best_move.rotation)) {
               move_is_illegal = false;
               continue;
            }
         } else if (m_ctx_ptr->game.can_place_tile_and_figure(best_move.x, best_move.y, best_move.rotation, m_ctx_ptr->game.tile_set()[m_ctx_ptr->game.move_index()], best_move.direction)) {
            move_is_illegal = false;
            continue;
         }
         spdlog::info("deep rl: selected incorrect move, running MCTS again");
      } while (move_is_illegal);
      if (m_ctx_ptr->leading_tread_id == std::this_thread::get_id()) {
         /*
            Trees merges here
            Only the leading thread does merging
         */
         // ctx_ptr->lck.lock(); // no need for lock
         spdlog::info("Worker: Finished !!!");
         m_ctx_ptr->node_with_best_move = node_with_best_move;
         m_ctx_ptr->move_readyness->m_data.resultReady = true;
         m_ctx_ptr->move_found->notify_one();
         // ctx_ptr->lck.unlock(); // no need for lock
      }
   }
   m_ctx_ptr->move_readyness->terminated = true;
   m_ctx_ptr->move_found->notify_one();
   spdlog::info("client_work: terminated");
}

void DeepRLPlayer::add_record(IGame &game, Node* node_with_best_move) {
   // std::vector<float> board_state(board_features_count(game.player_count(), g_max_moves));
   training::MoveNetworkRecord record;
   game.board_to_caffe_X(record.game_state);
   for (const auto& node : node_with_best_move->children()) {
      record.children_visits[encode_move(node->move())] = node->m_simulation_count / static_cast<float>(node_with_best_move->m_simulation_count);
   }
   record.player = m_ctx_ptr->player;
   game.training_data().emplace_back(std::move(record));
}

void DeepRLPlayer::make_move(IGame &game) noexcept {   
   spdlog::info("deep rl: preparing move");
   // rl::DataWrapper<rl::MoveReadyness> mrw{rl::MoveReadyness{false, false}};
      
   m_ctx_ptr->game = game;
   m_ctx_ptr->player = game.current_player();
   spdlog::debug("Player={}", game.current_player());
   m_ctx_ptr->move_readyness->m_data.dataReady = true;
   m_ctx_ptr->ready_to_move->notify_one();
   std::mutex mut;
   std::unique_lock<std::mutex> lck(mut);
   spdlog::info("deep rl: Waiting for move...");
   m_ctx_ptr->move_found->wait(lck, [this] {
      spdlog::info("deep rl: move_ready={}",
                   (m_ctx_ptr->move_readyness->m_data.resultReady ? "true" : "false"));
      return m_ctx_ptr->move_readyness->m_data.resultReady;
   });
   m_ctx_ptr->move_readyness->m_data.resultReady = false;
   
   Node* node_with_best_move = m_ctx_ptr->node_with_best_move;
   FullMove best_move = node_with_best_move->move();
   auto move = game.new_move(m_ctx_ptr->player);
   if (auto res = move->place_tile_at(best_move.x, best_move.y, best_move.rotation); !res.ok()) {
      spdlog::error("deep rl: selected tile placement is not feasible: {}", res.msg());
      return;
   }

   if (move->phase() == MovePhase::Done) {
      add_record(game, node_with_best_move);
      return;
   }
   
   if (best_move.ignored_figure) {
      if (auto res = move->ignore_figure(); !res.ok()) {
         spdlog::error("deep rl: cannot ignore figure at this location: {}", res.msg());
      }
      add_record(game, node_with_best_move);
      return;
   }

   if (auto res = move->place_figure(best_move.direction); !res.ok()) {
      spdlog::error("deep rl: error placing figure: {}", res.msg());
   }
   add_record(game, node_with_best_move);
}

}// namespace carcassonne::ai