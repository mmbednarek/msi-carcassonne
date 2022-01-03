#include <Carcassonne/AI/DeepRLPlayer.h>
// #define SPDLOG_FMT_EXTERNAL
#include <spdlog/spdlog.h>
#include <iostream>

namespace carcassonne::ai {

DeepRLPlayer::DeepRLPlayer(
   IGame &game,
   Player player,
   mb::size gpus,
   mb::size cpus )
    : m_player(player)
    , m_tree(game, player)
    , m_player_count(game.player_count())
    , m_gpus(gpus)
    , m_cpus(cpus) {
   spdlog::info("deep rl: initialising agent");
   game.on_next_move([this](IGame &game, Player player, FullMove last_move) {
      m_last_moves[static_cast<mb::size>(last_player(player, m_player_count))] = last_move;
      if (player != m_player)
         return;
      make_move(game);
   });
}

void rl::client_threads::client_work(unsigned cpu_id) {
   std::cout << "client Thread(" << cpu_id << ") " << std::this_thread::get_id() << " run on cpu " << cpu_id << std::endl;
   ctx_ptr->lck.lock();
   if (!ctx_ptr->leading_tread_assigned) {
      ctx_ptr->leading_tread_id = std::this_thread::get_id();
   }
   ctx_ptr->lck.unlock();
   
   auto tile = ctx_ptr->game.tile_set()[ctx_ptr->game.move_index()];
   ctx_ptr->lck.lock();
   g_trees.emplace(std::this_thread::get_id(), std::move(prepare_tree(ctx_ptr->game, ctx_ptr->player)));
   ctx_ptr->lck.unlock();
   FullMove best_move;
   bool move_is_illegal = true;
   do {
      rl::run_mcts(ctx_ptr, 1000, 0);
      best_move = rl::choose_move(ctx_ptr, game.move_index());
      m_last_moves[static_cast<int>(m_player)] = best_move;
      if (best_move.ignored_figure) {
         if (game.board().can_place_at(best_move.x, best_move.y, game.tile_set()[game.move_index()], best_move.rotation)) {
            move_is_illegal = false;
            continue;
         }
      } else if (game.can_place_tile_and_figure(best_move.x, best_move.y, best_move.rotation, game.tile_set()[game.move_index()], best_move.direction)) {
         move_is_illegal = false;
         continue;
      }
      spdlog::info("deep rl: selected incorrect move, running MCTS again");
   } while (move_is_illegal);

   if (ctx_ptr->leading_tread_id != std::this_thread::get_id()) return;
   /*
      Trees merges here
      Only the leading thread does merging
   */
   // ctx_ptr->lck.lock(); // no need for lock
   ctx_ptr->best_move = best_move;
   ctx_ptr->move_ready = true;
   cond_var->notify_one();
   // ctx_ptr->lck.unlock(); // no need for lock
}

std::unique_ptr<Tree> DeepRLPlayer::prepare_tree(const IGame &game, Player player) {
   Tree tree;
   Player current_player = player;
   NodeId node_id = 0;
   do {
      node_id = tree.find_node_by_move(node_id, m_last_moves[static_cast<mb::size>(player)]);
      if (node_id == 0)
         break;
      player = next_player(player, game.player_count());
   } while (player != current_player);

   if (node_id == 0) {
      spdlog::debug("deep rl: building MCTS tree from scratch");
      tree.reset(game, current_player);
      return std::make_unique<Tree>(std::move(tree));
   }

   spdlog::debug("deep rl: reusing existing MCTS tree node");
   tree.change_root(node_id);
   return std::make_unique<Tree>(std::move(tree));
}

void DeepRLPlayer::make_move(IGame &game) noexcept {
   unsigned trees_count = 1;
   std::cout << "cpus=" << std::thread::hardware_concurrency() << std::endl;
   
   spdlog::info("deep rl: preparing move");
   std::unique_ptr<rl::Context> ctx_ptr = std::make_unique<rl::Context>(game, m_player);
   
   std::shared_ptr<std::condition_variable> condVar =
           std::make_shared<std::condition_variable>();
   std::mutex mut;
   m_clients_pool = std::make_unique<rl::client_threads>(trees_count, ctx_ptr, condVar);
   spdlog::info("deep rl: Waiting for move...");
   std::unique_lock<std::mutex> lck(mut);
   condVar->wait(lck, [&my_que] {
      spdlog::info("deep rl: move_ready={}",
                   (ctx_ptr->move_ready ? "true" : "false"));
      return ctx_ptr->move_ready;
   });
   m_clients_pool->join_clients();
   
   FullMove best_move = ctx_ptr->best_move;
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