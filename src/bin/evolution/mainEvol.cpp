#include <Carcassonne/AI/HeuristicPlayer.h>
#include <Carcassonne/Game/Game.h>
#include <Carcassonne/Parameters.h>
#include <Evolution/DiffEvol.h>
// #include <boost/program_options.hpp>
#include <fmt/core.h>

constexpr auto g_population_size = 360;
constexpr auto g_generations_count = 200;
constexpr auto g_mutation_chance = 0.9;
constexpr auto g_cross_chance = .8;
constexpr auto g_mutation_rate_initial = 0.2;
constexpr auto g_mutation_rate_final = 0.05;
constexpr auto g_optimal_fitness = 300;

std::array<carcassonne::ai::HeuristicPlayer, 4> g_heuristic_players{
        carcassonne::ai::HeuristicPlayer(carcassonne::Player::Blue),
        carcassonne::ai::HeuristicPlayer(carcassonne::Player::Black),
        carcassonne::ai::HeuristicPlayer(carcassonne::Player::Red),
        carcassonne::ai::HeuristicPlayer(carcassonne::Player::Yellow),
};

auto make_objective_function(util::IRandomGenerator &rand, carcassonne::Parameters &base_params, evolution::Parameters &evo_params) {
   return [&evo_params,
           &base_params,
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

      // fmt::print("winner is {}", median_score > 0 ? "Blue! :)" : "Black :(");
      // fmt::print("\tFitness: {}\n", median_score);
      return median_score;
   };
}

constexpr bool player_range_ok(int count) {
   return count >= 0 && count <= 4;
}

int main(int argc, char **argv) {
   util::Random rand;
   carcassonne::Parameters params;

   evolution::Constraint constraint{
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
           .mutation_chance = g_mutation_chance,
           .cross_chance = g_cross_chance,
           .mutation_rate_initial = g_mutation_rate_initial,
           .mutation_rate_final = g_mutation_rate_final,
           .optimal_fitness = g_optimal_fitness,
   };

   auto objective_func = make_objective_function(rand, params, evo_params);
   auto result = evolution::FindOptimal(rand, objective_func, evo_params, constraint);

   // fmt::print("\nfinal result:");
   // fmt::print("\n  monastery_score:\t\t{}, ", result.monastery_score);
   // fmt::print("\n  grass_penalty:\t\t{}, ", result.grass_penalty);
   // fmt::print("\n  min_figure_count:\t\t{}, ", result.min_figure_count);
   // fmt::print("\n  grass_score:\t\t{}, ", result.grass_score);
   // fmt::print("\n  tile_type_score:\t\t{}, ", result.tile_type_score);
   // fmt::print("\n  tile_close_score:\t\t{}, ", result.tile_close_score);
   // fmt::print("\n  tile_open_score:\t\t{}, ", result.tile_open_score);
   // fmt::print("\n  ignore_figure_score_treshold:\t\t{}, \n", result.ignore_figure_score_threshold);
}
