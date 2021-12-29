#include <Carcassonne_GPU/AI/DeepRL.h>
#include <Carcassonne_GPU/AI/HeuristicPlayer.h>
#include <Carcassonne_GPU/AI/RandomPlayer.h>
#include <Carcassonne_GPU/AI/Tree.h>
#include <Util_GPU/Time.h>
#include <cassert>
#include <chrono>
#define SPDLOG_FMT_EXTERNAL
#include <spdlog/spdlog.h>
#include <random>

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

void simulate_random(Context &ctx, NodeId node_id) {
   auto parent_id = node_id;
   game::Game simulated_game{ctx.tree.node_at(node_id).game()};
   for (auto move_index = simulated_game->move_index(); move_index < g_max_moves; ++move_index) {
      auto current_player = simulated_game->current_player();
      auto full_move = g_random_players[static_cast<mb::size>(current_player)].make_move(*simulated_game);
      simulated_game->update(0);
      parent_id = ctx.tree.add_node(simulated_game->clone(), current_player, full_move, parent_id);
   }

   auto &leaf_node = ctx.tree.node_at(parent_id);
   auto winner = leaf_node.find_winner();
   leaf_node.mark_as_expanded();
   backpropagate(ctx, parent_id, winner);
}

static FullMove get_move(Context &ctx, IGame &game) {
   return ctx.network.do_move(game, game.tile_set()[game.move_index()], static_cast<float>(rand() % 1000) / 1000.0f);// rand is a hash
}

void simulate(Context &ctx, NodeId node_id) {
   auto parent_id = node_id;
   auto simulated_game = ctx.tree.node_at(node_id).game().clone();
   for (auto move_index = simulated_game->move_index(); move_index < g_max_moves; ++move_index) {
      auto current_player = simulated_game->current_player();
      auto full_move = get_move(ctx, *simulated_game);
      auto move = simulated_game->new_move(current_player);
      move->place_tile_at(full_move.x, full_move.y, full_move.rotation);
      move->place_figure(full_move.direction);
      simulated_game->update(0);

      parent_id = ctx.tree.add_node(simulated_game->clone(), current_player, full_move, parent_id);
   }

   auto &leaf_node = ctx.tree.node_at(parent_id);
   auto winner = leaf_node.find_winner();
   leaf_node.mark_as_expanded();
   backpropagate(ctx, parent_id, winner);
}

void backpropagate(Context &ctx, NodeId node_id, Player winner) {
   while (node_id != g_root_node) {
      auto &node = ctx.tree.node_at(node_id);
      node.propagate(winner);
      node_id = node.parent_id();
   }
   ctx.tree.node_at(g_root_node).propagate(winner);
}

void expand(Context &ctx, NodeId node_id) {
   int num_threads = 1024;
	const dim3 bdim(num_threads, 1);
	const dim3 gdim(1, 1, 1);
   if (ctx.tree.node_at(node_id).simulation_count() == 0) {
      auto start = util_GPU::unix_time();
      simulate(ctx, node_id);
      spdlog::debug("deep rl: simulation lasted {}ms", util_GPU::unix_time() - start);
   }

   auto &node_children = ctx.tree.node_at(node_id).children();
   if (node_children.empty())
      return;

   auto child_node = *node_children.begin();
   auto simulation_move = ctx.tree.node_at(child_node).move();

   auto &game = ctx.tree.node_at(node_id).game();
   const auto current_player = game.current_player();
   for (auto tile_location : game.moves()) {
      bool simulated_tile = false;
      if (tile_location.x == simulation_move.x && tile_location.y == simulation_move.y && tile_location.rotation == simulation_move.rotation) [[unlikely]] {
         simulated_tile = true;
      }

      game::Game game_clone{game};
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
         ctx.tree.add_node(std::move(game_clone_clone), current_player, full_move, node_id);
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
         ctx.tree.add_node(std::move(game_clone), current_player, full_move, node_id);
      }
   }

   ctx.tree.node_at(node_id).mark_as_expanded();
}

void run_selection(Context &ctx) {
   const auto rollout_count = ctx.tree.node_at(g_root_node).simulation_count();

   auto current_node_id = g_root_node;
   for (;;) {
      auto &current_node = ctx.tree.node_at(current_node_id);
      const auto &children = current_node.children();

      auto selected_child_it = std::max_element(
              children.begin(),
              children.end(),
              [&ctx, rollout_count](NodeId lhs, NodeId rhs) -> bool {
                 return ctx.tree.node_at(lhs).UCT1(rollout_count) < ctx.tree.node_at(rhs).UCT1(rollout_count);
              });
      assert(selected_child_it != children.end());

      const auto child_id = *selected_child_it;
      auto &child_node = ctx.tree.node_at(child_id);

      if (!child_node.expanded()) {
         expand(ctx, child_id);
         return;
      }

      if (child_node.children().empty()) {
         backpropagate(ctx, child_id, child_node.find_winning_player());
         return;
      }

      current_node_id = child_id;
   }
}

void run_mcts(Context &ctx, mb::i64 time_limit) {
   if (!ctx.tree.node_at(g_root_node).expanded()) {
      expand(ctx, g_root_node);
   }

   auto until = std::chrono::steady_clock::now() + std::chrono::milliseconds{time_limit};
   while (std::chrono::steady_clock::now() < until) {
      run_selection(ctx);
   }
}

FullMove choose_move(Context &ctx, int move_index, Player player) {
   auto &root_node = ctx.tree.node_at(g_root_node);
   const auto &children = root_node.children();
   auto max_sim_count_it = std::max_element(
           children.begin(),
           children.end(),
           [&ctx](NodeId lhs, NodeId rhs) -> bool {
              return ctx.tree.node_at(lhs).simulation_count() < ctx.tree.node_at(rhs).simulation_count();
           });
   auto max_sim_count = ctx.tree.node_at(*max_sim_count_it).simulation_count();

   auto selected = std::max_element(
           children.begin(), children.end(), [&ctx, player, max_sim_count](NodeId lhs, NodeId rhs) {
              auto lhs_sc = ctx.tree.node_at(lhs).simulation_count();
              auto rhs_sc = ctx.tree.node_at(rhs).simulation_count();
              if (lhs_sc != max_sim_count && rhs_sc == max_sim_count)
                 return true;
              if (rhs_sc != max_sim_count)
                 return false;
              return ctx.tree.node_at(lhs).player_wins(player) > ctx.tree.node_at(rhs).player_wins(player);
           });


   assert(selected != children.end());
   auto &node = ctx.tree.node_at(*selected);
   return node.move();
}

}// namespace carcassonne::ai::rl