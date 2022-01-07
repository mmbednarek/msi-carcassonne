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
    , m_move_readyness(std::make_shared<rl::DataWrapper<rl::MoveReadyness>>())
    , m_ready_to_move(std::make_shared<std::condition_variable>())
    , m_move_found(std::make_shared<std::condition_variable>())
    , m_player(player)
    , m_generator(generator)
    , m_ctx_ptr(std::make_unique<rl::Context>(  game,
                                                m_player,
                                                m_last_moves,
                                                workers_pool,
                                                m_move_readyness,
                                                m_ready_to_move,
                                                m_move_found) )
    , m_trees_count(trees_count)
    , m_clients_pool(std::make_unique<rl::client_threads>(m_trees_count, m_ctx_ptr) )
{
   spdlog::info("deep rl: initialising agent");
   std::cout << "cpus=" << std::thread::hardware_concurrency() << std::endl;
   game.on_next_move([this](IGame &game, Player player, FullMove last_move) {
      m_last_moves[static_cast<mb::size>(last_player(player, m_player_count))] = last_move;
      if (player != m_player)
         return;
      make_move(game);
   });
}

bool prepare_tree(std::unique_ptr<rl::Context> &ctx_ptr) {
   ctx_ptr->lck.lock();
   std::unique_ptr<Tree>& tree = ctx_ptr->trees[std::this_thread::get_id()];
   ctx_ptr->lck.unlock();
   if (nullptr == tree) return 1;
   NodePtr node = tree->root();
   auto player = ctx_ptr->player;
   do {
      node = tree->find_node_by_move(node, ctx_ptr->last_moves[static_cast<mb::size>(player)]);
      if (node == nullptr)
         break;
      player = next_player(player, ctx_ptr->game.player_count());
   } while (player != ctx_ptr->player);

   if (node == nullptr) {
      spdlog::debug("deep rl: building MCTS tree from scratch");
      tree->reset(ctx_ptr->game, ctx_ptr->player);
      return 0;
   }
   spdlog::debug("deep rl: reusing existing MCTS tree node");
   tree->change_root(node);
   return 0;
}

void rl::client_threads::client_work(unsigned cpu_id) {
   spdlog::debug("client_work: client run on cpu '{}'", cpu_id);
   std::this_thread::sleep_for(std::chrono::seconds(1));
   ctx_ptr->lck.lock();
   if (!ctx_ptr->leading_tread_assigned) {
      ctx_ptr->leading_tread_id = std::this_thread::get_id();
   }
   spdlog::debug("thread {} pushes tree", thread_name());
   ctx_ptr->trees.emplace(std::this_thread::get_id(), std::make_unique<Tree>(ctx_ptr->game, ctx_ptr->player));
   spdlog::debug("thread {} pushed tree", thread_name());
   ctx_ptr->lck.unlock();
   FullMove best_move;
   spdlog::debug("client_work: client '{}' started", cpu_id);
   while (true) {
      spdlog::info("client_work: client '{}' waiting...", cpu_id);
      std::unique_lock<std::mutex> lck(ctx_ptr->move_readyness->m_mutex);
      ctx_ptr->ready_to_move->wait(lck, [this] {
         spdlog::info("client_work: dataReady={}",
                      (ctx_ptr->move_readyness->m_data.dataReady ? "true" : "false"));
         return ctx_ptr->move_readyness->m_data.dataReady || ctx_ptr->move_readyness->terminate;
      });
      if (ctx_ptr->move_readyness->terminate) {
         spdlog::info("client_work: terminate");
         break;
      }
      spdlog::info("client_work: client '{}' Running !!!");
      if (prepare_tree(ctx_ptr)) {
         spdlog::debug("nullptr == tree");
         return;
      }
      bool move_is_illegal = true;
      do {
         rl::run_mcts(ctx_ptr, 0, 1600);
         best_move = rl::choose_move(ctx_ptr, ctx_ptr->game.move_index());
         ctx_ptr->last_moves[static_cast<int>(ctx_ptr->player)] = best_move;
         if (best_move.ignored_figure) {
            if (ctx_ptr->game.board().can_place_at(best_move.x, best_move.y, ctx_ptr->game.tile_set()[ctx_ptr->game.move_index()], best_move.rotation)) {
               move_is_illegal = false;
               continue;
            }
         } else if (ctx_ptr->game.can_place_tile_and_figure(best_move.x, best_move.y, best_move.rotation, ctx_ptr->game.tile_set()[ctx_ptr->game.move_index()], best_move.direction)) {
            move_is_illegal = false;
            continue;
         }
         spdlog::info("deep rl: selected incorrect move, running MCTS again");
      } while (move_is_illegal);
      if (ctx_ptr->leading_tread_id == std::this_thread::get_id()) {
         /*
            Trees merges here
            Only the leading thread does merging
         */
         // ctx_ptr->lck.lock(); // no need for lock
         spdlog::info("Worker: Finished !!!");
         ctx_ptr->best_move = best_move;
         ctx_ptr->move_readyness->m_data.dataReady = false;
         ctx_ptr->move_readyness->m_data.resultReady = true;
         ctx_ptr->move_found->notify_one();
         // ctx_ptr->lck.unlock(); // no need for lock
      }
   }
}

void DeepRLPlayer::make_move(IGame &game) noexcept {   
   spdlog::info("deep rl: preparing move");
   // rl::DataWrapper<rl::MoveReadyness> mrw{rl::MoveReadyness{false, false}};
      
   std::mutex mut;
   std::unique_lock<std::mutex> lck(mut);
   m_ctx_ptr->game = game;
   m_ctx_ptr->player = game.current_player();
   m_ctx_ptr->move_readyness->m_data.dataReady = true;
   spdlog::info("deep rl: Waiting for move...");
   m_ctx_ptr->move_found->wait(lck, [this] {
      spdlog::info("deep rl: move_ready={}",
                   (m_ctx_ptr->move_readyness->m_data.resultReady ? "true" : "false"));
      return m_ctx_ptr->move_readyness->m_data.resultReady;
   });
   
   FullMove best_move = m_ctx_ptr->best_move;
   auto move = game.new_move(m_player);
   if (auto res = move->place_tile_at(best_move.x, best_move.y, best_move.rotation); !res.ok()) {
      spdlog::error("deep rl: selected tile placement is not feasible: {}", res.msg());
      return;
   }

   if (move->phase() == MovePhase::Done)
      return;

   if (best_move.ignored_figure) {
      if (auto res = move->ignore_figure(); !res.ok()) {
         spdlog::error("deep rl: cannot ignore figure at this location: {}", res.msg());
      }
      return;
   }

   if (auto res = move->place_figure(best_move.direction); !res.ok()) {
      spdlog::error("deep rl: error placing figure: {}", res.msg());
   }
}

}// namespace carcassonne::ai