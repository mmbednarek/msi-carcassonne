#include <Carcassonne/AI/HeuristicPlayer.h>
#include <Carcassonne/AI/MCTS.h>
#include <Carcassonne/AI/RandomPlayer.h>
#include <Carcassonne/AI/Tree.h>
#include <Carcassonne/Game/Game.h>
#include <Util/Time.h>
#include <cassert>
#include <chrono>
#include <fmt/core.h>
#include <random>

namespace carcassonne::ai {

//util::CSVLogger g_mcts_logger("logs/mcts.csv",
//                              "move_number", "ai_wins", "best_utc", "node_sim_count", "rollout_count", "node_count");

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

void simulate_random(Tree &tree, NodePtr node_id) {
   auto parent_id = node_id;
   auto simulated_game = node_id->game().clone();
   for (auto move_index = simulated_game->move_index(); move_index < g_max_moves; ++move_index) {
      auto current_player = simulated_game->current_player();
      auto full_move = g_random_players[static_cast<mb::size>(current_player)].make_move(*simulated_game);
      simulated_game->update(0);
      parent_id = tree.add_node(rl::PoolGame(*simulated_game), current_player, full_move, 0.0, parent_id);
   }

   auto winner = parent_id->find_winner();
   parent_id->mark_as_expanded();
   backpropagate(tree, parent_id, winner);
}

void simulate(Tree &tree, NodePtr node_id) {
   auto parent_id = node_id;
   auto simulated_game = node_id->game().clone();
   for (auto move_index = simulated_game->move_index(); move_index < g_max_moves; ++move_index) {
      auto current_player = simulated_game->current_player();
      auto full_move = g_heuristic_players[static_cast<mb::size>(current_player)].make_move(*simulated_game).unwrap();
      simulated_game->update(0);

      parent_id = tree.add_node(rl::PoolGame(*simulated_game), current_player, full_move, 0.0, parent_id);
   }

   auto winner = parent_id->find_winner();
   parent_id->mark_as_expanded();
   backpropagate(tree, parent_id, winner);
}

void backpropagate(Tree &tree, NodePtr node_id, Player winner) {
   while (!node_id->is_root()) {
      node_id->propagate(winner);
      node_id = node_id->parent();
   }
   node_id->propagate(winner);
}

void expand(Tree &tree, NodePtr node_id, SimulationType simulation_type) {
   if (node_id->simulation_count() == 0) {
      switch (simulation_type) {
      case SimulationType::Heuristic: {
         auto start = util::unix_time();
         simulate(tree, node_id);
         fmt::print("simulation lasted {}ms\n", (util::unix_time() - start));
         break;
      }
      case SimulationType::Random:
         simulate_random(tree, node_id);
         break;
      }
   }

   auto &node_children = node_id->children();
   if (node_children.empty())
      return;

   const auto &child_node = *node_children.begin();
   auto simulation_move = child_node->move();

   auto &game = node_id->game();
   const auto current_player = game.current_player();
   for (auto tile_location : game.moves()) {
      bool simulated_tile = false;
      if (tile_location.x == simulation_move.x && tile_location.y == simulation_move.y && tile_location.rotation == simulation_move.rotation) [[unlikely]] {
         simulated_tile = true;
      }

      rl::PoolGame game_clone(game);
      auto move = game_clone->new_move(current_player);
      move->place_tile(tile_location);

      for (Direction figure_move : game_clone->figure_placements(tile_location.x, tile_location.y)) {
         if (simulated_tile && figure_move == simulation_move.direction && !simulation_move.ignored_figure) [[unlikely]]
            continue;

         rl::PoolGame game_clone_clone(game_clone);

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
         tree.add_node(std::move(game_clone_clone), current_player, full_move, 0.0, node_id);
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
         tree.add_node(std::move(game_clone), current_player, full_move, 0.0, node_id);
      }
   }

   node_id->mark_as_expanded();
}

void run_selection(Tree &tree, SimulationType sim_type) {
   const auto rollout_count = tree.root()->simulation_count();

   auto current_node = tree.root();
   for (;;) {
      const auto &children = current_node->children();

      auto selected_child_it = std::max_element(
              children.begin(),
              children.end(),
              [rollout_count](NodeUPtr lhs, NodeUPtr &rhs) -> bool {
                 return lhs->UCT1(rollout_count) < rhs->UCT1(rollout_count);
              });
      assert(selected_child_it != children.end());

      auto child_node = selected_child_it->get();

      if (!child_node->expanded()) {
         expand(tree, child_node, sim_type);
         return;
      }

      if (child_node->children().empty()) {
         backpropagate(tree, child_node, child_node->find_winning_player());
         return;
      }

      current_node = child_node;
   }
}

void run_mcts(Tree &tree, mb::i64 time_limit, SimulationType sim_type) {
   if (!tree.root()->expanded()) {
      expand(tree, tree.root(), sim_type);
   }

   auto until = std::chrono::steady_clock::now() + std::chrono::milliseconds{time_limit};
   while (std::chrono::steady_clock::now() < until) {
      run_selection(tree, sim_type);
   }
}

FullMove choose_move(Tree &tree, Player player) {
   auto root_node = tree.root();
   const auto &children = root_node->children();
   auto max_sim_count_it = std::max_element(
           children.begin(),
           children.end(),
           [&tree](NodeUPtr lhs, NodeUPtr rhs) -> bool {
              return lhs->simulation_count() < rhs->simulation_count();
           });
   auto max_sim_count = max_sim_count_it->get()->simulation_count();

   auto selected = std::max_element(
           children.begin(), children.end(), [&tree, player, max_sim_count](NodeUPtr lhs, NodeUPtr rhs) {
              auto lhs_sc = lhs->simulation_count();
              auto rhs_sc = rhs->simulation_count();
              if (lhs_sc != max_sim_count && rhs_sc == max_sim_count)
                 return true;
              if (rhs_sc != max_sim_count)
                 return false;
              return lhs->player_wins(player) > rhs->player_wins(player);
           });

   assert(selected != children.end());
   const auto &node = selected->get();
   return node->move();
}

}// namespace carcassonne::ai