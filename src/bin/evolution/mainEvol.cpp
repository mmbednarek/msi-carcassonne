#include <Carcassonne/AI/HeuristicPlayer.h>
#include <Carcassonne/Game/Game.h>
#include <Carcassonne/Parameters.h>
#include <Evolution/DiffEvol.h>
#include <fmt/core.h>

constexpr auto g_population_size = 36;
constexpr auto g_generations_count = 100;
constexpr auto g_cross_chance = .2;
constexpr auto g_mutation_rate_initial = 0.8;
constexpr auto g_mutation_rate_final = 0.05;

constexpr auto g_switching_operations_count = 10;

std::array<carcassonne::ai::HeuristicPlayer, 4> g_heuristic_players{
        carcassonne::ai::HeuristicPlayer(carcassonne::Player::Black),
        carcassonne::ai::HeuristicPlayer(carcassonne::Player::Blue),
        carcassonne::ai::HeuristicPlayer(carcassonne::Player::Red),
        carcassonne::ai::HeuristicPlayer(carcassonne::Player::Yellow),
};

auto make_objective_function(util::IRandomGenerator &rand, carcassonne::Parameters &base_params) {
   return [&base_params,
           &rand](const evolution::Variables &vars) -> double {
      carcassonne::Parameters params;
      params.monastery_score = vars.monastery_score;
      params.grass_penalty = vars.grass_penalty;
      params.min_figure_count = vars.min_figure_count;
      params.grass_score = vars.grass_score;
      params.tile_type_score = vars.tile_type_score;
      params.tile_close_score = vars.tile_close_score;
      params.tile_open_score = vars.tile_open_score;
      params.ignore_figure_score_threshold = vars.ignore_figure_score_threshold;

      auto games_count = 10;
      std::vector<int> scores(games_count);
      for (mb::u64 seed = 0; seed < games_count; seed++) {
         carcassonne::game::Game game(2, seed);
         auto simulated_game = game;
         for (auto move_index = simulated_game.move_index(); move_index < carcassonne::g_max_moves; ++move_index) {
            auto current_player = simulated_game.current_player();
            g_heuristic_players[static_cast<mb::size>(current_player)].make_move(simulated_game, current_player == carcassonne::Player::Blue ? params : base_params).unwrap();
            simulated_game.update(0);
         }
         auto score_it = simulated_game.scores().begin();
         auto score = 0;
         if (score_it != simulated_game.scores().end()) {
            score = score_it->score;
            auto winner = score_it->player;
            ++score_it;
            if (score_it != simulated_game.scores().end()) {
               score -= score_it->score;
            }
            if (winner != carcassonne::Player::Blue) {
               score *= -1;
            }
            scores[seed] = score;
         }
      }
      size_t n = games_count / 2;
      std::nth_element(scores.begin(), scores.begin() + n, scores.end());
      auto median_score = scores[games_count / 2];
      return median_score;
   };
}

constexpr bool player_range_ok(int count) {
   return count >= 0 && count <= 4;
}

int main(int argc, char **argv) {
   util::Random rand;
   carcassonne::Parameters params;

   evolution::ParamsRanges constraint{
           {1, 5000}, // moastery_score
           {1, 100},  // grass_penalty
           {2, 7},    // min_figure_count
           {1, 100},  // grass_score
           {1, 100},  // tile_type_score
           {1, 50000},// tile_close_score
           {1, 1000}, // tile_open_score
           {0, 10000},// ignore_figure_score_treshold
   };

   evolution::ParamsRanges initial_params{
           {1, 5000}, // moastery_score
           {1, 100},  // grass_penalty
           {2, 7},    // min_figure_count
           {1, 100},  // grass_score
           {1, 100},  // tile_type_score
           {1, 50000},// tile_close_score
           {1, 1000}, // tile_open_score
           {0, 10000},// ignore_figure_score_treshold
   };

   evolution::Parameters evo_params{
           .population_size = g_population_size,
           .generations_count = g_generations_count,
           .cross_chance = g_cross_chance,
           .mutation_rate_initial = g_mutation_rate_initial,
           .mutation_rate_final = g_mutation_rate_final,
   };

   for (auto i = 0; i < g_switching_operations_count; ++i) {
      fmt::print("\nSwitch nr {}:", i);
      auto objective_func = make_objective_function(rand, params);
      auto result = evolution::FindOptimal(rand, objective_func, constraint, initial_params, evo_params);
      params = result;
      fmt::print("\nfinal result:");
      fmt::print("\tmoastery_score: {},  ", result.monastery_score);
      fmt::print("\tgrass_penalty: {}, ", result.grass_penalty);
      fmt::print("\tmin_figure_count: {}, ", result.min_figure_count);
      fmt::print("\tgrass_score: {}, ", result.grass_score);
      fmt::print("\ttile_type_score: {}, ", result.tile_type_score);
      fmt::print("\ttile_open_score: {}, ", result.tile_open_score);
      fmt::print("\tignore_figure_score_treshold: {}, ", result.ignore_figure_score_threshold);
      fmt::print("\ttile_close_score: {}\n", result.tile_close_score);
   }
}
