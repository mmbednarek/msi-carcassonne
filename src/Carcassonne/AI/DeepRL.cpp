#include <Carcassonne/AI/DeepRL.h>
#include <Carcassonne/AI/HeuristicPlayer.h>
#include <Carcassonne/AI/RandomPlayer.h>
#include <Carcassonne/AI/Tree.h>
#include <Util/CSVLogger.h>
#include <Util/Time.h>
#include <cassert>
#include <chrono>
#include <spdlog/spdlog.h>
#include <random>
#include <limits>
#include <future>
#include <vector>
#include <thread>

namespace carcassonne::ai::rl {

std::array<HeuristicPlayer, 4> g_heuristic_players{
        HeuristicPlayer(Player::Black),
        HeuristicPlayer(Player::Blue),
        HeuristicPlayer(Player::Red),
        HeuristicPlayer(Player::Yellow),
};

std::random_device g_rand_device;
std::mt19937 g_random_gen(g_rand_device());

std::array<RandomPlayer<>, 4> g_random_players{
        RandomPlayer(g_random_gen, Player::Black),
        RandomPlayer(g_random_gen, Player::Blue),
        RandomPlayer(g_random_gen, Player::Red),
        RandomPlayer(g_random_gen, Player::Yellow),
};

void simulate_random(std::unique_ptr<rl::Context> &ctx_ptr, NodeId node_id) {
   auto parent_id = node_id;
   auto simulated_game = ctx_ptr->tree.node_at(node_id).game().clone();
   for (auto move_index = simulated_game->move_index(); move_index < g_max_moves; ++move_index) {
      auto current_player = simulated_game->current_player();
      auto full_move = g_random_players[static_cast<mb::size>(current_player)].make_move(*simulated_game);
      simulated_game->update(0);
      parent_id = ctx_ptr->tree.add_node(simulated_game->clone(), current_player, full_move, parent_id);
   }

   auto &leaf_node = ctx_ptr->tree.node_at(parent_id);
   auto winner = leaf_node.find_winner();
   leaf_node.mark_as_expanded();
   backpropagate(ctx_ptr, parent_id, winner);
}

static FullMove get_move(const std::unique_ptr<IGame> &game) {
#ifdef MEASURE_TIME
   fmt::print("\n{}\n", g_max_moves - game->move_index());
#endif
   std::hash<std::thread::id> hasher;
   auto tid = hasher(std::this_thread::get_id());
   FullMove mv = g_networks[std::this_thread::get_id()]->do_move(game, static_cast<float>(rand_r(&tid) % 1000) / 1000.0f);// rand is a hash
   if (mv.ignored_figure) {
      if (!game->board().can_place_at(mv.x, mv.y, game->tile_set()[game->move_index()], mv.rotation)) {
         spdlog::error("deep rl, get_move(): INCORRECT TILE PLACEMENT 52 !!!");
      }
   } else if (!game->can_place_tile_and_figure(mv.x, mv.y, mv.rotation, game->tile_set()[game->move_index()], mv.direction)) {
      spdlog::error("deep rl, get_move(): INCORRECT FIGURE PLACEMRNT 55 !!!");
   }
   return mv;
}

void simulate(std::unique_ptr<rl::Context> &ctx_ptr, NodeId node_id) {

      auto start = util::unix_time();
      NodeId parent_id = node_id;
      
      auto simulate_lambda = [&ctx_ptr, &parent_id](){
         auto simulated_game = ctx_ptr->tree.node_at(parent_id).game().clone();
         for (auto move_index = simulated_game->move_index(); move_index < g_max_moves; ++move_index) {
#ifdef MEASURE_TIME
            auto start_move = util::unix_time();
#endif
            auto current_player = simulated_game->current_player();
            auto full_move = get_move(simulated_game);
            auto move = simulated_game->new_move(current_player);
            move->place_tile_at(full_move.x, full_move.y, full_move.rotation);
            move->place_figure(full_move.direction);
            simulated_game->update(0);

            ctx_ptr->lck.lock();
            parent_id = ctx_ptr->tree.add_node(simulated_game->clone(), current_player, full_move, parent_id);
            ctx_ptr->lck.unlock();
#ifdef MEASURE_TIME
            spdlog::debug("deep rl: move lasted {}ms", util::unix_time() - start_move);
#endif
         }
      };
      ctx_ptr->workers_pool->submit(simulate_lambda);
      // simulate(ctx_ptr, parent_id);
      spdlog::debug("deep rl: simulation lasted {}ms", util::unix_time() - start);

      auto &leaf_node = ctx_ptr->tree.node_at(parent_id);
      auto winner = leaf_node.find_winner();
      leaf_node.mark_as_expanded();
      backpropagate(ctx_ptr, parent_id, winner);

//    auto simulated_game = ctx_ptr->tree.node_at(node_id).game().clone();
//    for (auto move_index = simulated_game->move_index(); move_index < g_max_moves; ++move_index) {
// #ifdef MEASURE_TIME
//       auto start_move = util::unix_time();
// #endif
//       auto current_player = simulated_game->current_player();
//       auto full_move = get_move(simulated_game);
//       auto move = simulated_game->new_move(current_player);
//       move->place_tile_at(full_move.x, full_move.y, full_move.rotation);
//       move->place_figure(full_move.direction);
//       simulated_game->update(0);

//       ctx_ptr->lck.lock();
//       node_id = ctx_ptr->tree.add_node(simulated_game->clone(), current_player, full_move, node_id);
//       ctx_ptr->lck.unlock();
// #ifdef MEASURE_TIME
//       spdlog::debug("deep rl: move lasted {}ms", util::unix_time() - start_move);
// #endif

//    }
}

void backpropagate(std::unique_ptr<rl::Context> &ctx_ptr, NodeId node_id, Player winner) {
   while (node_id != g_root_node) {
      auto &node = ctx_ptr->tree.node_at(node_id);
      node.propagate(winner);
      node_id = node.parent_id();
   }
   ctx_ptr->tree.node_at(g_root_node).propagate(winner);
}

void expand(std::unique_ptr<rl::Context> &ctx_ptr, NodeId node_id) {

   auto &node_children = ctx_ptr->tree.node_at(node_id).children();
   if (node_children.empty())
      return;

   auto child_node = *node_children.begin();
   auto simulation_move = ctx_ptr->tree.node_at(child_node).move();

   auto &game = ctx_ptr->tree.node_at(node_id).game();
   const auto current_player = game.current_player();
   for (auto tile_location : game.moves()) {
      if (!game.board().can_place_at(tile_location.x, tile_location.y, game.tile_set()[game.move_index()], tile_location.rotation)) {
         spdlog::error("deep rl, expand(): INCORRECT TILE PLACEMENT 114!!!");
      }
      bool simulated_tile = false;
      if (tile_location.x == simulation_move.x && tile_location.y == simulation_move.y && tile_location.rotation == simulation_move.rotation) [[unlikely]] {
         simulated_tile = true;
      }
      std::array<bool, g_directions.size()> feasible_dirs;
      for (int i = 0; i < g_directions.size(); ++i) {
         if (game.can_place_tile_and_figure(tile_location.x, tile_location.y, tile_location.rotation, game.tile_set()[game.move_index()], g_directions[i])) {
            feasible_dirs[i] = true;
         }
      }
      auto game_clone = game.clone();
      auto move = game_clone->new_move(current_player);
      move->place_tile(tile_location);
      if (game.player_figure_count(game.current_player()) > 0) {
         for (Direction figure_move : game_clone->figure_placements(tile_location.x, tile_location.y)) {
            if (simulated_tile && figure_move == simulation_move.direction && !simulation_move.ignored_figure) [[unlikely]]
               continue;
            // if (!game.can_place_tile_and_figure(tile_location.x, tile_location.y, tile_location.rotation, game.tile_set()[game.move_index()], figure_move)) {
            //    spdlog::info("deep rl: INCORRECT MOVE 139!!!");
            //    continue;
            // }
            auto game_clone_clone = game_clone->clone();
            {
               auto move_clone = move->clone(*game_clone_clone);
               move_clone->place_figure(figure_move);
            }
            game_clone_clone->update(0);

            const auto full_move = FullMove{
                  .x = tile_location.x,
                  .y = tile_location.y,
                  .rotation = tile_location.rotation,
                  .ignored_figure = false,
                  .direction = figure_move,
            };
            ctx_ptr->tree.add_node(std::move(game_clone_clone), current_player, full_move, node_id);
         }
      }
      if (simulated_tile && simulation_move.ignored_figure)
         continue;

      move->ignore_figure();
      game_clone->update(0);
      {
         const auto full_move = FullMove{
                 .x = tile_location.x,
                 .y = tile_location.y,
                 .rotation = tile_location.rotation,
                 .ignored_figure = true,
         };
         // if (!game.board().can_place_at(full_move.x, full_move.y, game.tile_set()[game.move_index()], full_move.rotation)) {
         //    spdlog::info("deep rl: INCORRECT MOVE 174 !!!");
         //    continue;
         // }
         ctx_ptr->tree.add_node(std::move(game_clone), current_player, full_move, node_id);
      }
   }

   ctx_ptr->tree.node_at(node_id).mark_as_expanded();
}

void run_selection(std::unique_ptr<rl::Context> &ctx_ptr) {
   const auto rollout_count = ctx_ptr->tree.node_at(g_root_node).simulation_count();
   std::vector<NodeId> nodes_to_be_simulated;

   auto current_node_id = g_root_node;
   for (;;) {
      auto &current_node = ctx_ptr->tree.node_at(current_node_id);
      const auto &current_children = current_node.children();

      if (!current_children.empty())
      auto selected_child_it = current_children.end();
      selected_child_it = std::max_element(
              current_children.begin(),
              current_children.end(),
              [&ctx_ptr, rollout_count](NodeId lhs, NodeId rhs) -> bool {
                 return ctx_ptr->tree.node_at(lhs).UCT1(rollout_count) < ctx_ptr->tree.node_at(rhs).UCT1(rollout_count);
              });
      assert(selected_child_it != current_children.end());

      const auto selected_child_id = *selected_child_it;
      auto &selected_child_node = ctx_ptr->tree.node_at(selected_child_id);
      assert(selected_child_node.simulated()); // selected must has been silmulated
      expand(ctx_ptr, selected_child_id);
      children = selected_child_node.children();
      if (!children.empty()) {
         spdlog::info("deep rl, run_selection: bottom of the tree reached");
         backpropagate(ctx_ptr, selected_child_id, selected_child_node.find_winning_player());
         return;
      }
      std::vector<std::thread> threads{children.size()};
      for (int i = 0; i < children.size(); ++i) {
         threads[i] = std::thread(simulate, std::ref(ctx_ptr), children[i]);
      }
      std::for_each(threads.begin(),threads.end(), std::mem_fn(&std::thread::join));
      
      if (!selected_child_node.expanded()) {
         if (!selected_child_node.simulated()) {
            nodes_to_be_simulated.push_back(selected_child_id);
         } else {
            expand(ctx_ptr, selected_child_id);
            break;
         }
         return;
      }
      
      if (selected_child_node.children().empty()) 

      current_node_id = child_id;
   }
}

void run_mcts(std::unique_ptr<rl::Context> &ctx_ptr, mb::i64 time_limit, mb::i64 runs_limit) {
   std::unique_lock<std::mutex> lck_init(ctx_ptr->tree.m_mutex_nodes);
   if (!ctx_ptr->tree.node_at(g_root_node).expanded()) {
      expand(ctx_ptr, g_root_node);
   }
   lck_init.unlock();
   if (time_limit == 0) time_limit = std::numeric_limits<mb::i64>::max();
   if (runs_limit == 0) runs_limit = std::numeric_limits<mb::i64>::max();

   auto until = std::chrono::steady_clock::now() + std::chrono::milliseconds{time_limit};
   ctx_ptr->lck.lock();
   for (mb::i64 i = 0; std::chrono::steady_clock::now() < until && i < runs_limit; ++i) {
      run_selection(ctx_ptr);
   }
}

FullMove choose_move(std::unique_ptr<rl::Context> &ctx_ptr, int move_index, Player player) {
   auto &root_node = ctx_ptr->tree.node_at(g_root_node);
   const auto &children = root_node.children();
   // 
   // choose max element
   auto max_sim_count_it = std::max_element(
           children.begin(),
           children.end(),
           [&ctx_ptr](NodeId lhs, NodeId rhs) -> bool {
              return ctx_ptr->tree.node_at(lhs).simulation_count() < ctx_ptr->tree.node_at(rhs).simulation_count();
           });
   auto max_sim_count = ctx_ptr->tree.node_at(*max_sim_count_it).simulation_count();

   auto selected = std::max_element(
           children.begin(), children.end(), [&ctx_ptr, player, max_sim_count](NodeId lhs, NodeId rhs) {
              auto lhs_sc = ctx_ptr->tree.node_at(lhs).simulation_count();
              auto rhs_sc = ctx_ptr->tree.node_at(rhs).simulation_count();
              if (lhs_sc != max_sim_count && rhs_sc == max_sim_count)
                 return true;
              if (rhs_sc != max_sim_count)
                 return false;
              return ctx_ptr->tree.node_at(lhs).player_wins(player) > ctx_ptr->tree.node_at(rhs).player_wins(player);
           });


   assert(selected != children.end());
   auto &node = ctx_ptr->tree.node_at(*selected);
   return node.move();
}

}// namespace carcassonne::ai::rl