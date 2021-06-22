#include <Carcassonne/AI/HeuristicPlayer.h>
#include <Carcassonne/AI/RandomPlayer.h>
#include <Carcassonne/AI/MCTS.h>
#include <Carcassonne/AI/Tree.h>
#include <cassert>
#include <chrono>
#include <fmt/core.h>
#include <Util/CSVLogger.h>
#include <random>

namespace carcassonne::ai {

util::CSVLogger g_mcts_logger("logs/mcts.csv",
                              "move_number", "ai_wins", "best_utc", "node_sim_count", "rollout_count", "node_count");

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

void simulate_random(Tree &tree, NodeId node_id) {
   auto parent_id = node_id;
   auto simulated_game = tree.node_at(node_id).game().clone();
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

void simulate(Tree &tree, NodeId node_id) {
   auto parent_id = node_id;
   auto simulated_game = tree.node_at(node_id).game().clone();
   for (auto move_index = simulated_game->move_index(); move_index < g_max_moves; ++move_index) {
      auto current_player = simulated_game->current_player();
      auto full_move = g_heuristic_players[static_cast<mb::size>(current_player)].make_move(*simulated_game).unwrap();
      simulated_game->update(0);

      parent_id = tree.add_node(simulated_game->clone(), current_player, full_move, parent_id);
   }

   auto &leaf_node = tree.node_at(parent_id);
   auto winner = leaf_node.find_winner();
   leaf_node.mark_as_expanded();
   backpropagate(tree, parent_id, winner);
}

void backpropagate(Tree &tree, NodeId node_id, Player winner) {
   while (node_id != g_root_node) {
      auto &node = tree.node_at(node_id);
      node.propagate(winner);
      node_id = node.parent_id();
   }
   tree.node_at(g_root_node).propagate(winner);
}

void expand(Tree &tree, NodeId node_id, SimulationType simulation_type) {
   if (tree.node_at(node_id).simulation_count() == 0) {
      switch (simulation_type) {
      case SimulationType::Heuristic:
         simulate(tree, node_id);
         break;
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

         const auto full_move = FullMove{
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
         const auto full_move = FullMove{
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

void run_selection(Tree &tree, SimulationType sim_type) {
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

void run_mcts(Tree &tree, mb::i64 time_limit, SimulationType sim_type) {
   if (!tree.node_at(g_root_node).expanded()) {
      expand(tree, g_root_node, sim_type);
   }

   auto until = std::chrono::steady_clock::now() + std::chrono::milliseconds{time_limit};
   while (std::chrono::steady_clock::now() < until) {
      run_selection(tree, sim_type);
   }
}

FullMove choose_move(Tree &tree, int move_index, Player player) {
   auto &root_node = tree.node_at(g_root_node);
   const auto &children = root_node.children();
   auto selected_child_it = std::max_element(
           children.begin(),
           children.end(),
           [&tree](NodeId lhs, NodeId rhs) -> bool {
              return tree.node_at(lhs).simulation_count() < tree.node_at(rhs).simulation_count();
           });
   assert(selected_child_it != children.end());
   auto &node = tree.node_at(*selected_child_it);
   g_mcts_logger.log(move_index, node.player_wins(player), node.UCT1(root_node.simulation_count()), node.simulation_count(), root_node.simulation_count(), tree.node_count());
   return node.move();
}

}// namespace carcassonne::ai