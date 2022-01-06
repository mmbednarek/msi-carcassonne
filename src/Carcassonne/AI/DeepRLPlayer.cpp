#include <Carcassonne/AI/DeepRLPlayer.h>
// #define SPDLOG_FMT_EXTERNAL
#include <spdlog/spdlog.h>
#include <iostream>

namespace carcassonne::ai {

DeepRLPlayer::DeepRLPlayer(
   IGame &game,
   Player player )
    : m_player(player)
    , m_player_count(game.player_count()) {
   spdlog::info("deep rl: initialising agent");
   std::cout << "cpus=" << std::thread::hardware_concurrency() << std::endl;
   game.on_next_move([this](IGame &game, Player player, FullMove last_move) {
      m_last_moves[static_cast<mb::size>(last_player(player, m_player_count))] = last_move;
      if (player != m_player)
         return;
      make_move(game);
   });
}

std::unique_ptr<Tree> prepare_tree(std::unique_ptr<rl::Context> &ctx_ptr) {
   Tree tree(ctx_ptr->game, ctx_ptr->player);
   Player current_player = ctx_ptr->player;
   NodeId node_id = 0;
   do {
      node_id = tree.find_node_by_move(node_id, ctx_ptr->last_moves[static_cast<mb::size>(ctx_ptr->player)]);
      if (node_id == 0)
         break;
      ctx_ptr->player = next_player(ctx_ptr->player, ctx_ptr->game.player_count());
   } while (ctx_ptr->player != current_player);

   if (node_id == 0) {
      spdlog::debug("deep rl: building MCTS tree from scratch");
      tree.reset(ctx_ptr->game, current_player);
      return std::make_unique<Tree>(std::move(tree));
   }

   spdlog::debug("deep rl: reusing existing MCTS tree node");
   tree.change_root(node_id);
   return std::make_unique<Tree>(std::move(tree));
}

void rl::client_threads::client_work(unsigned cpu_id) {
   spdlog::debug("client_work: client Thread({}) run on cpu {}", cpu_id);
   ctx_ptr->lck.lock();
   if (!ctx_ptr->leading_tread_assigned) {
      ctx_ptr->leading_tread_id = std::this_thread::get_id();
   }
   ctx_ptr->lck.unlock();
   auto tree = prepare_tree(ctx_ptr);
   ctx_ptr->lck.lock();
   spdlog::debug("thread {} pushes tree", thread_name());
   ctx_ptr->trees.emplace(std::this_thread::get_id(), std::move(tree));
   spdlog::debug("thread {} pushed tree", thread_name());
   ctx_ptr->lck.unlock();
   FullMove best_move;
   bool move_is_illegal = true;
   do {
      rl::run_mcts(ctx_ptr, 1000, 0);
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

void DeepRLPlayer::make_move(IGame &game) noexcept {
   unsigned trees_count = 1;
   
   spdlog::info("deep rl: preparing move");
   std::unique_ptr<rl::Context> ctx_ptr = std::make_unique<rl::Context>(game, m_player, m_last_moves);
   
   std::shared_ptr<std::condition_variable> condVar =
           std::make_shared<std::condition_variable>();
   std::mutex mut;
   m_clients_pool = std::make_unique<rl::client_threads>(trees_count, ctx_ptr, condVar);
   spdlog::info("deep rl: Waiting for move...");
   std::unique_lock<std::mutex> lck(mut);
   condVar->wait(lck, [&ctx_ptr] {
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