#include <Carcassonne/AI/DeepRL.h>
#include <Carcassonne/AI/HeuristicPlayer.h>
#include <Carcassonne/AI/RandomPlayer.h>
#include <Carcassonne/AI/Tree.h>
#include <Carcassonne/Decoder/Decoder.h>
#include <Util/CSVLogger.h>
#include <Util/Time.h>
#include <algorithm>
#include <cassert>
#include <chrono>
#include <iterator>
#include <spdlog/spdlog.h>
#include <random>
#include <limits>
#include <future>
#include <vector>
#include <thread>
#include <span>

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

void backpropagate(
        NodePtr node,
        Player winner,
        std::unique_ptr<Tree> &tree) {
   while (!node->is_root()) {
      node->propagate(winner);
      node = node->parent();
   }
   node->propagate(winner);
}

void backpropagate_state_value(
        NodePtr node,
        const float &state_value,
        std::unique_ptr<Tree> &tree) {
   while (!node->is_root()) {
      node->propagate_state_value(state_value);
      node = node->parent();
   }
   node->propagate_state_value(state_value);
}

std::tuple<std::span<float>, float> get_probabilities(std::unique_ptr<rl::Context> &ctx_ptr, NodePtr node) {
   std::vector<float> neuron_input;
   IGame& game = node->game();
   game.board_to_caffe_X(neuron_input);
   std::promise<std::tuple<std::span<float>, float>> promise;
   util::DataWithPromise< std::vector<float>, std::tuple<std::span<float>, float> > data{ &promise, &neuron_input };
   ctx_ptr->workers_pool->submit(data);
   return data.promise->get_future().get();
}

void expand(std::unique_ptr<rl::Context> &ctx_ptr, NodePtr node) {
   std::unique_ptr<Tree>& tree = ctx_ptr->trees[std::this_thread::get_id()];
   auto &game = node->game();
   if (std::distance(game.moves().begin(), game.moves().end()) == 0) {
      spdlog::error("terminal 1!");
      // backpropagate_state_value(node, -100.0, tree);
      node->mark_as_terminal();
      node->mark_as_expanded();
      return;
   }
   const auto current_player = game.current_player();
   auto [probabilities, state_value] = get_probabilities(ctx_ptr, node);
   if (node->game().move_index() >= g_max_moves) {
      backpropagate_state_value(node, state_value, tree);
      return;
   }
   for (auto tile_location : game.moves()) {
      if (!game.board().can_place_at(tile_location.x, tile_location.y, game.tile_set()[game.move_index()], tile_location.rotation)) {
         spdlog::error("deep rl, expand(): INCORRECT TILE PLACEMENT 114!!!");
      }
      std::array<bool, g_directions.size()> feasible_dirs{};
      for (mb::size i = 0; i < g_directions.size(); ++i) {
         if (game.can_place_tile_and_figure(tile_location.x, tile_location.y, tile_location.rotation, game.tile_set()[game.move_index()], g_directions[i])) {
            feasible_dirs[i] = true;
         }
      }
      auto game_clone = game.clone();
      auto move = game_clone->new_move(current_player);
      move->place_tile(tile_location);
      if (game.player_figure_count(game.current_player()) > 0) {
         for (Direction figure_move : game_clone->figure_placements(tile_location.x, tile_location.y)) {
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
            tree->add_node(std::move(game_clone_clone), current_player, full_move, probabilities[encode_move(full_move)], node);
         }
      }
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
         const int probability_index = (tile_location.y * g_board_width + tile_location.x) * tile_location.rotation * 10;
         node->add_child(std::move(game_clone), current_player, full_move, probabilities[probability_index]);
      }
   }
   if (node->children().size() == 0) {
      spdlog::error("terminal 2!");
      // backpropagate_state_value(node, -100.0, tree);
      node->mark_as_terminal();
      if (nullptr != node->parent()) {
         node->parent()->mark_as_terminal();
      }
      return;
   }
   backpropagate_state_value(node, state_value, tree);
   node->mark_as_expanded();
}

void run_selection(std::unique_ptr<rl::Context> &ctx_ptr) {
   auto &tree = ctx_ptr->trees[std::this_thread::get_id()];
   // const auto rollout_count = tree->root()->simulation_count();
   auto current_node = tree->root();
   for (;;) {
      const auto &children = current_node->children();
      if (!children.empty()) {
         auto selected_child_it = children.end();
         selected_child_it = std::max_element(
                 children.begin(),
                 children.end(),
                 [](NodeUPtr lhs, NodeUPtr rhs) -> bool {
                    if (rhs->is_terminal()) return false;
                    if (lhs->is_terminal()) return true;
                    return lhs->PUCT() < rhs->PUCT();
                 });
         assert(selected_child_it != children.end());
         current_node = selected_child_it->get();
         continue;
      }
      assert(!current_node->is_terminal());
      assert(current_node->children().empty());
      expand(ctx_ptr, current_node);
      // launch_simulations(ctx_ptr, current_node_id);
      return;
   }
}

void run_mcts(std::unique_ptr<rl::Context> &ctx_ptr, mb::i64 time_limit, mb::i64 runs_limit) {
   auto &tree = ctx_ptr->trees[std::this_thread::get_id()];
   tree->lck.lock();
   if (std::distance(tree->root()->game().moves().begin(), tree->root()->game().moves().end()) == 0) {
      spdlog::error("terminal 0!, seed={}", ctx_ptr->game.seed());
      tree->root()->game().mutable_tile_set()[tree->root()->game().move_index()] = 1;
      ctx_ptr->game.mutable_tile_set()[tree->root()->game().move_index()] = 1;
   }
   if (!tree->root()->expanded()) {
      expand(ctx_ptr, tree->root());
   }
   ctx_ptr->trees[std::this_thread::get_id()]->lck.unlock();
   if (time_limit == 0) time_limit = std::numeric_limits<mb::i64>::max();
   if (runs_limit == 0) runs_limit = std::numeric_limits<mb::i64>::max();

   auto until = std::chrono::steady_clock::now() + std::chrono::milliseconds{time_limit};
   for (mb::i64 i = 0; std::chrono::steady_clock::now() < until && i < runs_limit; ++i) {
      run_selection(ctx_ptr);
   }
}

Node* choose_move(std::unique_ptr<rl::Context> &ctx_ptr) {
   Player player = ctx_ptr->player;
   auto root_node = ctx_ptr->trees[std::this_thread::get_id()]->root();
   const auto &children = root_node->children();
   assert(children.size() != 0);
   // 
   // choose max element
   auto max_sim_count_it = std::max_element(
           children.begin(),
           children.end(),
           [](NodeUPtr lhs, NodeUPtr rhs) -> bool {
              return lhs->simulation_count() < rhs->simulation_count();
           });
   auto max_sim_count = max_sim_count_it->get()->simulation_count();

   auto selected = std::max_element(
           children.begin(), children.end(), [player, max_sim_count](NodeUPtr lhs, NodeUPtr rhs) {
              auto lhs_sc = lhs->simulation_count();
              auto rhs_sc = rhs->simulation_count();
              if (lhs_sc != max_sim_count && rhs_sc == max_sim_count)
                 return true;
              if (rhs_sc != max_sim_count)
                 return false;
              return lhs->player_wins(player) > rhs->player_wins(player);
           });

   if (selected == children.end()) {
      spdlog::error("game.seed={}", ctx_ptr->game.seed());
      fmt::print("Tileset:\n");
      for (int i = 0; i < ctx_ptr->game.tile_set().size(); ++i)
         fmt::print("{}\n", static_cast<mb::u8>(ctx_ptr->game.tile_set()[i] ) );
      spdlog::error("game.tile={}", static_cast<mb::u8>(ctx_ptr->game.tile_set()[ctx_ptr->game.move_index() ] ) );
      spdlog::error("game.move_index={}", ctx_ptr->game.move_index() );
      assert(false);
   }
   assert(selected != children.end());
   return selected->get();
}

}// namespace carcassonne::ai::rl