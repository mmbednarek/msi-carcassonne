#include <Carcassonne/AI/HeuristicPlayer.h>
#include <Carcassonne/AI/MCTS.h>
#include <Carcassonne/AI/Tree.h>
#include <cassert>
#include <chrono>
#include <fmt/core.h>
#include <mb/result.h>

namespace carcassonne::ai {

std::array<HeuristicPlayer, 4> g_heuristic_players{
        HeuristicPlayer(Player::Black),
        HeuristicPlayer(Player::Blue),
        HeuristicPlayer(Player::Red),
        HeuristicPlayer(Player::Yellow),
};

mb::result<FullMove> find_non_idiotic(Tree &tree, Player player) {
   auto &children = tree.node_at(g_root_node).children();
   auto move_it = std::find_if(children.begin(), children.end(), [&tree, player](NodeId child_id) {
      return tree.node_at(child_id).player_wins(player) > 0;
   });
   if (move_it == children.end()) {
      return mb::error("no winning moves found");
   }

   auto &node = tree.node_at(*move_it);
   fmt::print("move id: {}, wins: {}, vc: {}\n", node.game().move_index(), node.player_wins(player), node.simulation_count());
   return node.move();
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

void expand(Tree &tree, NodeId node_id) {
   if (tree.node_at(node_id).simulation_count() == 0) {
      simulate(tree, node_id);
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

void run_selection(Tree &tree) {
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
         expand(tree, child_id);
         return;
      }

      if (child_node.children().empty()) {
         return;
      }

      current_node_id = child_id;
   }
}

void run_mcts(Tree &tree, mb::i64 time_limit) {
   if (!tree.node_at(g_root_node).expanded()) {
      expand(tree, g_root_node);
   }

   auto until = std::chrono::steady_clock::now() + std::chrono::milliseconds{time_limit};
   while (std::chrono::steady_clock::now() < until) {
      run_selection(tree);
   }
}

FullMove choose_move(Tree &tree) {
   auto &root_node = tree.node_at(g_root_node);
   const auto &children = root_node.children();
   auto selected_child_it = std::max_element(
           children.begin(),
           children.end(),
           [&tree, rollout_count = root_node.simulation_count()](NodeId lhs, NodeId rhs) -> bool {
              return tree.node_at(lhs).UCT1(rollout_count) < tree.node_at(rhs).UCT1(rollout_count);
           });
   assert(selected_child_it != children.end());
   auto &node = tree.node_at(*selected_child_it);
   fmt::print("win ratio: {}, sim count: {}, root sim count: {}, nodes: {}\n", node.win_ratio(), node.simulation_count(), root_node.simulation_count(), tree.node_count());
   return node.move();
}

}// namespace carcassonne::ai