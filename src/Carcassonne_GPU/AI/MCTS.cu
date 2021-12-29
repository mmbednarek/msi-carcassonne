#ifdef __CUDACC__
#define DEVHOST __device__ __host__
#else
#define DEVHOST
#endif

#include <Carcassonne_GPU/AI/HeuristicPlayer.h>
#include <Carcassonne_GPU/AI/RandomPlayer.h>
#include <Carcassonne_GPU/AI/MCTS.h>
#include <Carcassonne_GPU/AI/Tree.h>
#include <cassert>
#include <chrono>
#include <fmt/core.h>
#include <Util_GPU/Time.h>
#include <random>

namespace carcassonne::ai {

DEVHOST std::array<HeuristicPlayer, 4> g_heuristic_players{
        HeuristicPlayer(Player::Black),
        HeuristicPlayer(Player::Blue),
        HeuristicPlayer(Player::Red),
        HeuristicPlayer(Player::Yellow),
};

DEVHOST std::random_device g_rand_device;
DEVHOST std::mt19937 g_random_gen(g_rand_device());

DEVHOST std::array<RandomPlayer<>, 4> g_random_players{
        RandomPlayer(g_random_gen, Player::Black),
        RandomPlayer(g_random_gen, Player::Blue),
        RandomPlayer(g_random_gen, Player::Red),
        RandomPlayer(g_random_gen, Player::Yellow),
};

DEVHOST DEVHOST void simulate_random(Tree &tree, NodeId node_id) {
   auto parent_id = node_id;
   game::Game simulated_game{tree.node_at(node_id).game()};
   for (auto move_index = simulated_game->move_index(); move_index < g_max_moves; ++move_index) {
      auto current_player = simulated_game->current_player();
      auto full_move = g_random_players[static_cast<mb::size>(current_player)].make_move(*simulated_game);
      simulated_game->update(0);
      parent_id = tree.add_node(simulated_game->clone(), current_player, full_move, parent_id);
   }

   auto &leaf_node = tree.node_at(parent_id);
   auto winner = leaf_node.find_winner();
   leaf_node.mark_as_expanded();
   backpropagate(tree, parent_id, winner);
}

DEVHOST void simulate(Tree &tree, NodeId node_id) {
   auto parent_id = node_id;
   game::Game simulated_game{tree.node_at(node_id).game()};
   for (auto move_index = simulated_game->move_index(); move_index < g_max_moves; ++move_index) {
      auto current_player = simulated_game->current_player();
      auto full_move = g_heuristic_players[static_cast<mb::size>(current_player)].make_move(*simulated_game);
      simulated_game->update(0);

      parent_id = tree.add_node(simulated_game->clone(), current_player, full_move, parent_id);
   }

   auto &leaf_node = tree.node_at(parent_id);
   auto winner = leaf_node.find_winner();
   leaf_node.mark_as_expanded();
   backpropagate(tree, parent_id, winner);
}

DEVHOST void backpropagate(Tree &tree, NodeId node_id, Player winner) {
   while (node_id != g_root_node) {
      auto &node = tree.node_at(node_id);
      node.propagate(winner);
      node_id = node.parent_id();
   }
   tree.node_at(g_root_node).propagate(winner);
}

DEVHOST void expand(Tree &tree, NodeId node_id, SimulationType simulation_type) {
   if (tree.node_at(node_id).simulation_count() == 0) {
      switch (simulation_type) {
      case SimulationType::Heuristic: {
         auto start = util_GPU::unix_time();
         simulate(tree, node_id);
         fmt::print("simulation lasted {}ms\n", (util_GPU::unix_time() - start));
         break;
      }
      case SimulationType::Random:
         simulate_random(tree, node_id);
         break;
      }
   }

   auto &node_children = tree.node_at(node_id).children();
   if (node_children.empty())
      return;

   auto child_node = *node_children.begin();
   auto simulation_move = tree.node_at(child_node).move();

   auto &game = tree.node_at(node_id).game();
   const auto current_player = game.current_player();
   for (auto tile_location : game.moves()) {
      bool simulated_tile = false;
      if (tile_location.x == simulation_move.x && tile_location.y == simulation_move.y && tile_location.rotation == simulation_move.rotation) [[unlikely]] {
         simulated_tile = true;
      }

      auto game_clone = game.clone();
      auto move = game_clone->new_move(current_player);
      move->place_tile(tile_location);

      for (Direction figure_move : game_clone->figure_placements(tile_location.x, tile_location.y)) {
         if (simulated_tile && figure_move == simulation_move.direction && !simulation_move.ignored_figure) [[unlikely]]
            continue;

         auto game_clone_clone = game_clone->clone();

         {
            auto move_clone = move->clone(*game_clone_clone);
            move_clone->place_figure(figure_move);
         }
         game_clone_clone->update(0);

         const auto full_move = game::FullMove{
                 .x = tile_location.x,
                 .y = tile_location.y,
                 .rotation = tile_location.rotation,
                 .ignored_figure = false,
                 .direction = figure_move,
         };
         tree.add_node(std::move(game_clone_clone), current_player, full_move, node_id);
      }

      if (simulated_tile && simulation_move.ignored_figure)
         continue;

      move->ignore_figure();
      game_clone->update(0);
      {
         const auto full_move = game::FullMove{
                 .x = tile_location.x,
                 .y = tile_location.y,
                 .rotation = tile_location.rotation,
                 .ignored_figure = true,
         };
         tree.add_node(std::move(game_clone), current_player, full_move, node_id);
      }
   }

   tree.node_at(node_id).mark_as_expanded();
}

DEVHOST void run_selection(Tree &tree, SimulationType sim_type) {
   const auto rollout_count = tree.node_at(g_root_node).simulation_count();

   auto current_node_id = g_root_node;
   for (;;) {
      auto &current_node = tree.node_at(current_node_id);
      const auto &children = current_node.children();

      auto selected_child_it = std::max_element(
              children.begin(),
              children.end(),
              [&tree, rollout_count](NodeId lhs, NodeId rhs) -> bool {
                 return tree.node_at(lhs).UCT1(rollout_count) < tree.node_at(rhs).UCT1(rollout_count);
              });
      assert(selected_child_it != children.end());

      const auto child_id = *selected_child_it;
      auto &child_node = tree.node_at(child_id);

      if (!child_node.expanded()) {
         expand(tree, child_id, sim_type);
         return;
      }

      if (child_node.children().empty()) {
         backpropagate(tree, child_id, child_node.find_winning_player());
         return;
      }

      current_node_id = child_id;
   }
}

DEVHOST void run_mcts(Tree &tree, mb::i64 time_limit, SimulationType sim_type) {
   if (!tree.node_at(g_root_node).expanded()) {
      expand(tree, g_root_node, sim_type);
   }

   auto until = std::chrono::steady_clock::now() + std::chrono::milliseconds{time_limit};
   while (std::chrono::steady_clock::now() < until) {
      run_selection(tree, sim_type);
   }
}

DEVHOST game::FullMove choose_move(Tree &tree, int move_index, Player player) {
   auto &root_node = tree.node_at(g_root_node);
   const auto &children = root_node.children();
   auto max_sim_count_it = std::max_element(
           children.begin(),
           children.end(),
           [&tree](NodeId lhs, NodeId rhs) -> bool {
             return tree.node_at(lhs).simulation_count() < tree.node_at(rhs).simulation_count();
           });
   auto max_sim_count = tree.node_at(*max_sim_count_it).simulation_count();

   auto selected = std::max_element(
           children.begin(), children.end(), [&tree, player, max_sim_count](NodeId lhs, NodeId rhs) {
             auto lhs_sc = tree.node_at(lhs).simulation_count();
             auto rhs_sc = tree.node_at(rhs).simulation_count();
             if (lhs_sc != max_sim_count && rhs_sc == max_sim_count)
                return true;
             if (rhs_sc != max_sim_count)
                return false;
             return tree.node_at(lhs).player_wins(player) > tree.node_at(rhs).player_wins(player);
           });

   assert(selected != children.end());
   auto &node = tree.node_at(*selected);
   return node.move();
}

}// namespace carcassonne::ai