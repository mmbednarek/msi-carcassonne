#include <Carcassonne/AI/HeuristicPlayer.h>
#include <Carcassonne/Game/Game.h>
#include <Carcassonne/Parameters.h>
#include <Evolution/DiffEvol.h>
#include <fmt/core.h>

constexpr auto g_population_size = 32;
constexpr auto g_generations_count = 120;
constexpr auto g_cross_chance_initial = .1;
constexpr auto g_cross_chance_final = .5;
constexpr auto g_mutation_rate_initial = .8;
constexpr auto g_mutation_rate_final = 0.2;

constexpr auto g_switching_operations_count = 200;

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

     auto games_count = 5;
     std::vector<int> scores(games_count);
     auto seed_skipped = 0;
     for (mb::u64 i = seed_skipped; i < games_count + seed_skipped; ++i) {
        mb::u64 seed;
         if (i == 2) {
           seed = i + games_count;
        } else {
           seed = i;
        }
        carcassonne::game::Game game(2, seed);
         auto simulated_game = game;
         for (auto move_index = simulated_game.move_index(); move_index < carcassonne::g_max_moves; ++move_index) {
            auto current_player = simulated_game.current_player();
            auto move_to_ignore = g_heuristic_players[static_cast<mb::size>(current_player)].make_move(simulated_game, current_player == carcassonne::Player::Blue ? params : base_params).unwrap();
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
            scores[i - seed_skipped] = score;
         }
      }
     auto min_score_it = std::min_element(scores.begin(), scores.end());
//     std::nth_element(scores.begin(), scores.begin() + mid, scores.end());
//     size_t mid = games_count / 2;
//     auto median_score = scores[mid];
//     auto max_score_it = std::max_element(scores.begin(), scores.end());
     return *min_score_it;
   };
}

constexpr bool player_range_ok(int count) {
   return count >= 0 && count <= 4;
}

int main(int argc, char **argv) {
   util::Random rand;
   carcassonne::Parameters params;

   evolution::ParamsRanges constraint{
           {1, 10000}, // moastery_score
           {1, 10000},  // grass_penalty
           {1, 7},    // min_figure_count
           {1, 10000},  // grass_score
           {1, 10000},  // tile_type_score
           {1, 10000},// tile_close_score
           {1, 10000}, // tile_open_score
   };

   evolution::ParamsRanges initial_params{
           {1000, 10000}, // moastery_score
           {1, 1000},  // grass_penalty
           {1, 3},    // min_figure_count
           {1, 1000},  // grass_score
           {1, 1000},  // tile_type_score
           {1, 1000},// tile_close_score
           {1, 1000}, // tile_open_score
   };

   evolution::Parameters evo_params{
           .population_size = g_population_size,
           .generations_count = g_generations_count,
           .cross_chance_initial = g_cross_chance_initial,
           .cross_chance_final = g_cross_chance_final,
           .mutation_rate_initial = g_mutation_rate_initial,
           .mutation_rate_final = g_mutation_rate_final,
   };

   auto log_switch = fmt::output_file("heuristic_params_per_switch.csv");
   log_switch.print("switching_players_count,MeanFitness,moastery_score,grass_penalty,min_figure_count,grass_score,tile_type_score,tile_open_score,tile_close_score\n");
   auto log_all = fmt::output_file("all_heuristic_params.csv");
   log_all.print("iteration,fitness,moastery_score,grass_penalty,min_figure_count,grass_score,tile_type_score,tile_open_score,tile_close_score\n");
   auto log_gen = fmt::output_file("gen_heuristic_params.csv");
   log_gen.print("generation,MeanFitness,MaxFitness,moastery_score,grass_penalty,min_figure_count,grass_score,tile_type_score,tile_open_score,tile_close_score\n");
   fmt::print("generation,MeanFitness,MaxFitness,moastery_score,grass_penalty,min_figure_count,grass_score,tile_type_score,tile_open_score,tile_close_score\n");

   for (auto i = 0; i < g_switching_operations_count; ++i) {
      //fmt::print("\nSwitch nr {}:", i);
      double optimal_fitness;
      auto objective_func = make_objective_function(rand, params);
      std::tie(optimal_fitness, params) = evolution::FindOptimal(rand, objective_func, constraint, initial_params, evo_params, i, log_all, log_gen);
      initial_params = evolution::ParamsRanges {
              {static_cast<int>(params.monastery_score / (1. - 1. / g_mutation_rate_initial)), static_cast<int>(params.monastery_score * (1. + 1. / g_mutation_rate_initial))}, // moastery_score
              {static_cast<int>(params.grass_penalty / (1. - 1. / g_mutation_rate_initial)), static_cast<int>(params.grass_penalty * (1. + 1. / g_mutation_rate_initial))}, // grass_penalty
              {static_cast<int>(params.min_figure_count / (1. - 1. / g_mutation_rate_initial)), static_cast<int>(params.min_figure_count * (1. + 1. / g_mutation_rate_initial))}, // min_figure_count
              {static_cast<int>(params.grass_score / (1. - 1. / g_mutation_rate_initial)), static_cast<int>(params.grass_score * (1. - 1. / g_mutation_rate_initial))}, // grass_score
              {static_cast<int>(params.tile_type_score / (1. - 1. / g_mutation_rate_initial)), static_cast<int>(params.tile_type_score * (1. + 1. / g_mutation_rate_initial))}, // tile_type_score
              {static_cast<int>(params.tile_close_score / (1. - 1. / g_mutation_rate_initial)), static_cast<int>(params.tile_close_score * (1. + 1. / g_mutation_rate_initial))}, // tile_close_score
              {static_cast<int>(params.tile_open_score / (1. - 1. / g_mutation_rate_initial)), static_cast<int>(params.tile_open_score * (1. + 1. / g_mutation_rate_initial))}, // tile_open_score
      };
//      fmt::print("\nfinal result:");
//      fmt::print("\tmoastery_score: {},  ", params.monastery_score);
//      fmt::print("\tgrass_penalty: {}, ", params.grass_penalty);
//      fmt::print("\tmin_figure_count: {}, ", params.min_figure_count);
//      fmt::print("\tgrass_score: {}, ", params.grass_score);
//      fmt::print("\ttile_type_score: {}, ", params.tile_type_score);
//      fmt::print("\ttile_open_score: {}, ", params.tile_open_score);
//      fmt::print("\ttile_close_score: {}\n", params.tile_close_score);

//      fmt::print("{},", i);
//      fmt::print("{},", optimal_fitness);
//      fmt::print("{},", params.monastery_score);
//      fmt::print("{},", params.grass_penalty);
//      fmt::print("{},", params.min_figure_count);
//      fmt::print("{},", params.grass_score);
//      fmt::print("{},", params.tile_type_score);
//      fmt::print("{},", params.tile_open_score);
//      fmt::print("{}\n", params.tile_close_score);

      log_switch.print("{},", i);
      log_switch.print("{},", optimal_fitness);
      log_switch.print("{},", params.monastery_score);
      log_switch.print("{},", params.grass_penalty);
      log_switch.print("{},", params.min_figure_count);
      log_switch.print("{},", params.grass_score);
      log_switch.print("{},", params.tile_type_score);
      log_switch.print("{},", params.tile_open_score);
      log_switch.print("{}\n", params.tile_close_score);
   }
   log_gen.close();
   log_all.close();
   log_switch.close();
}
