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

std::map<std::thread::id, std::unique_ptr<Network>> g_networks;

static FullMove get_move(const std::unique_ptr<IGame> &game) {
#ifdef MEASURE_TIME
   fmt::print("\n{}\n", g_max_moves - game->move_index());
#endif
   std::hash<std::thread::id> hasher;
   unsigned tid = hasher(std::this_thread::get_id());
   spdlog::debug("thread {} acquires network", hasher(std::this_thread::get_id()));
   std::unique_ptr<Network> &net_ptr = g_networks[std::this_thread::get_id()];
   spdlog::debug("thread {} acquired network", hasher(std::this_thread::get_id()));
   if (nullptr == net_ptr) {
      spdlog::debug("thread {} nullptr == net_ptr");
      return FullMove{};
   }
   if (nullptr == game) {
      spdlog::debug("thread {} nullptr == game");
      return FullMove{};
   }
   spdlog::debug("thread {} nullptr != game");
   FullMove mv = net_ptr->do_move(game, static_cast<float>(rand_r(&tid) % 1000) / 1000.0f, std::this_thread::get_id());// rand is a hash

   if (mv.ignored_figure) {
      if (!game->board().can_place_at(mv.x, mv.y, game->tile_set()[game->move_index()], mv.rotation)) {
         spdlog::error("deep rl, get_move(): INCORRECT TILE PLACEMENT 52 !!!");
      }
   } else if (!game->can_place_tile_and_figure(mv.x, mv.y, mv.rotation, game->tile_set()[game->move_index()], mv.direction)) {
      spdlog::error("deep rl, get_move(): INCORRECT FIGURE PLACEMRNT 55 !!!");
   }
   return mv;
}

void simulate(std::unique_ptr<rl::Context> &ctx_ptr, NodeId node_id, std::unique_ptr<Tree> &tree) {
   std::hash<std::thread::id> hasher;
   unsigned tid = hasher(std::this_thread::get_id());
   spdlog::debug("thread {}: carcassonne::ai::rl::simulate", tid);

   auto start = util::unix_time();

   auto simulate_lambda = [&ctx_ptr, &node_id, &tree]() {
      std::hash<std::thread::id> hasher;
      unsigned tid = hasher(std::this_thread::get_id());
      spdlog::debug("thread {}: simulate_lambda", tid);
      auto simulated_game = tree->node_at(node_id).game().clone();
      spdlog::debug("thread {}: ok1", tid);
      for (auto move_index = simulated_game->move_index(); move_index < g_max_moves; ++move_index) {
#ifdef MEASURE_TIME
         auto start_move = util::unix_time();
#endif
      spdlog::debug("thread {}: simulate_lambda: ok2", tid);
         auto current_player = simulated_game->current_player();
      spdlog::debug("thread {}: simulate_lambda: ok3", tid);
         auto full_move = get_move(simulated_game);
      spdlog::debug("thread {}: simulate_lambda: ok4", tid);
         auto move = simulated_game->new_move(current_player);
      spdlog::debug("thread {}: simulate_lambda: ok5", tid);
         move->place_tile_at(full_move.x, full_move.y, full_move.rotation);
      spdlog::debug("thread {}: simulate_lambda: ok6", tid);
         move->place_figure(full_move.direction);
      spdlog::debug("thread {}: simulate_lambda: ok7", tid);
         simulated_game->update(0);
      spdlog::debug("thread {}: simulate_lambda: ok8", tid);
#ifdef MEASURE_TIME
         spdlog::debug("deep rl: move lasted {}ms", util::unix_time() - start_move);
#endif
      spdlog::debug("thread {}: simulate_lambda: ok9", tid);
      }
      spdlog::debug("thread {}: simulate_lambda: ok10", tid);
      auto max_score_it = std::max_element(simulated_game->scores().begin(), simulated_game->scores().end(), [](PlayerScore lhs, PlayerScore rhs) {
         return lhs.score < rhs.score;
      });
      spdlog::debug("thread {}: simulate_lambda: ok11", tid);
      return max_score_it->player;
   };
   spdlog::debug("ok1");
   auto winner_future = ctx_ptr->workers_pool.submit(simulate_lambda);
   spdlog::debug("ok2");
   Player winner = winner_future.get();
   spdlog::debug("SUCCESS!");
   tree->node_at(node_id).mark_as_simulated();
   spdlog::debug("deep rl: simulation lasted {}ms", util::unix_time() - start);

   tree->lck.lock();
   backpropagate(node_id, winner, tree);
   tree->lck.unlock();
}

void backpropagate(
        NodeId node_id,
        Player winner,
        std::unique_ptr<Tree> &tree) {
   while (node_id != g_root_node_id) {
      auto &node = tree->node_at(node_id);
      node.propagate(winner);
      node_id = node.parent_id();
   }
   tree->node_at(g_root_node_id).propagate(winner);
}

void expand(std::unique_ptr<rl::Context> &ctx_ptr, const NodeId node_id) {
   auto &game = ctx_ptr->trees[std::this_thread::get_id()]->node_at(node_id).game();
   const auto current_player = game.current_player();
   for (auto tile_location : game.moves()) {
      if (!game.board().can_place_at(tile_location.x, tile_location.y, game.tile_set()[game.move_index()], tile_location.rotation)) {
         spdlog::error("deep rl, expand(): INCORRECT TILE PLACEMENT 114!!!");
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
            ctx_ptr->trees[std::this_thread::get_id()]->add_node(std::move(game_clone_clone), current_player, full_move, node_id);
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
         ctx_ptr->trees[std::this_thread::get_id()]->add_node(std::move(game_clone), current_player, full_move, node_id);
      }
   }
   ctx_ptr->trees[std::this_thread::get_id()]->node_at(node_id).mark_as_expanded();
}

void launch_simulations(std::unique_ptr<rl::Context> &ctx_ptr, const NodeId node_id) {
   Node &node = ctx_ptr->trees[std::this_thread::get_id()]->node_at(node_id);
   auto &children = node.children();
   auto size = node.children().size();
   if (0 == size) {
      spdlog::info("deep rl, run_selection: bottom of the tree reached");
      return;
   }
   std::vector<std::thread> threads{size};
   for (int i = 0; i < size; ++i) {
      threads[i] = std::thread(
              simulate,
              std::ref(ctx_ptr),
              children[i],
              std::ref(ctx_ptr->trees[std::this_thread::get_id()]));
   }
   std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
}

void run_selection(std::unique_ptr<rl::Context> &ctx_ptr) {
   const auto rollout_count = ctx_ptr->trees[std::this_thread::get_id()]->node_at(g_root_node_id).simulation_count();
   auto current_node_id = g_root_node_id;
   for (;;) {
      Node &current_node = ctx_ptr->trees[std::this_thread::get_id()]->node_at(current_node_id);
      const auto &children = current_node.children();

      if (!children.empty()) {
         auto selected_child_it = children.end();
         selected_child_it = std::max_element(
                 children.begin(),
                 children.end(),
                 [&ctx_ptr, rollout_count](NodeId lhs, NodeId rhs) -> bool {
                    return ctx_ptr->trees[std::this_thread::get_id()]->node_at(lhs).UCT1(rollout_count) < ctx_ptr->trees[std::this_thread::get_id()]->node_at(rhs).UCT1(rollout_count);
                 });
         assert(selected_child_it != children.end());
         current_node_id = *selected_child_it;
         continue;
      }
      assert(current_node.simulated());// selected must has been silmulated
      assert(current_node.children().empty());
      expand(ctx_ptr, current_node_id);
      launch_simulations(ctx_ptr, current_node_id);
      return;
   }
}

void run_mcts(std::unique_ptr<rl::Context> &ctx_ptr, mb::i64 time_limit, mb::i64 runs_limit) {
   std::hash<std::thread::id> hasher;
   spdlog::debug("thread {} acquires tree", hasher(std::this_thread::get_id()));
   auto &tree = ctx_ptr->trees[std::this_thread::get_id()];
   spdlog::debug("thread {} acquired tree", hasher(std::this_thread::get_id()));
   if (ctx_ptr->trees[std::this_thread::get_id()] == nullptr) {
      spdlog::debug("ctx_ptr->trees[std::this_thread::get_id()] == nullptr 2");
      return;
   }
   tree->lck.lock();
   if (!ctx_ptr->trees[std::this_thread::get_id()]->node_at(g_root_node_id).expanded()) {
      expand(ctx_ptr, g_root_node_id);
      launch_simulations(ctx_ptr, g_root_node_id);
   }
   ctx_ptr->trees[std::this_thread::get_id()]->lck.unlock();
   if (time_limit == 0) time_limit = std::numeric_limits<mb::i64>::max();
   if (runs_limit == 0) runs_limit = std::numeric_limits<mb::i64>::max();

   auto until = std::chrono::steady_clock::now() + std::chrono::milliseconds{time_limit};
   for (mb::i64 i = 0; std::chrono::steady_clock::now() < until && i < runs_limit; ++i) {
      run_selection(ctx_ptr);
   }
}

FullMove choose_move(std::unique_ptr<rl::Context> &ctx_ptr, int move_index) {
   Player player = ctx_ptr->player;
   auto &root_node = ctx_ptr->trees[std::this_thread::get_id()]->node_at(g_root_node_id);
   const auto &children = root_node.children();
   // 
   // choose max element
   auto max_sim_count_it = std::max_element(
           children.begin(),
           children.end(),
           [&ctx_ptr](NodeId lhs, NodeId rhs) -> bool {
              return ctx_ptr->trees[std::this_thread::get_id()]->node_at(lhs).simulation_count() < ctx_ptr->trees[std::this_thread::get_id()]->node_at(rhs).simulation_count();
           });
   auto max_sim_count = ctx_ptr->trees[std::this_thread::get_id()]->node_at(*max_sim_count_it).simulation_count();

   auto selected = std::max_element(
           children.begin(), children.end(), [&ctx_ptr, player, max_sim_count](NodeId lhs, NodeId rhs) {
              auto lhs_sc = ctx_ptr->trees[std::this_thread::get_id()]->node_at(lhs).simulation_count();
              auto rhs_sc = ctx_ptr->trees[std::this_thread::get_id()]->node_at(rhs).simulation_count();
              if (lhs_sc != max_sim_count && rhs_sc == max_sim_count)
                 return true;
              if (rhs_sc != max_sim_count)
                 return false;
              return ctx_ptr->trees[std::this_thread::get_id()]->node_at(lhs).player_wins(player) > ctx_ptr->trees[std::this_thread::get_id()]->node_at(rhs).player_wins(player);
           });


   assert(selected != children.end());
   auto &node = ctx_ptr->trees[std::this_thread::get_id()]->node_at(*selected);
   return node.move();
}

}// namespace carcassonne::ai::rl