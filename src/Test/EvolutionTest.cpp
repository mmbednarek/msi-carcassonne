#include <gtest/gtest.h>
#include <Evolution/DiffEvol.h>
#include <Util/IRandomGenerator.h>


TEST(Evolution, Simple) {
   constexpr auto g_population_size = 36;
   constexpr auto g_generations_count = 200;
   constexpr auto g_mutation_chance = 0.9;
   constexpr auto g_cross_chance = .8;
   constexpr auto g_mutation_rate_initial = 0.2;
   constexpr auto g_mutation_rate_final = 0.05;
   constexpr auto g_optimal_fitness = 300;

   util::Random r;

   auto objective_function = [](const evolution::Variables &vars) -> int {
      return vars.monastery_score; //score_diff;
   };

   evolution::Constraint constraint{
           {1, 5000},  // moastery_score
           {1, 100},    // grass_penalty
           {2, 7},      // min_figure_count
           {1, 100},    // grass_score
           {1, 100},    // tile_type_score
           {1, 50000},  // tile_close_score
           {1, 1000},   // tile_open_score
           {0, 10000},   // ignore_figure_score_treshold
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

   auto result = evolution::FindOptimal(r, objective_function, evo_params, constraint);
   ASSERT_GE(result.monastery_score, 1);
   ASSERT_GE(result.grass_penalty, 1);
   ASSERT_GE(result.min_figure_count, 1);
   ASSERT_GE(result.grass_score, 1);
   ASSERT_GE(result.tile_type_score, 1);
   ASSERT_GE(result.tile_close_score, 1);
   ASSERT_GE(result.tile_open_score, 1);
   ASSERT_GE(result.ignore_figure_score_threshold, 0);
}
