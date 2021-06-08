#include <Evolution/DiffEvol.h>

namespace evolution {

constexpr auto g_population_size = 12;
constexpr auto g_iteration_count = 12;
constexpr auto g_mutation = 0.8;
constexpr auto g_cross = 0.2;

template <typename T>
constexpr T mutate(T min, T max, T a, T b, T c) {
   return std::clamp(a + static_cast<T>(g_mutation * (b - c)), min, max);
}

Variables FindOptimal(util::IRandomGenerator &rand, const ObjectiveFunction &objective_function, evolution::Parameters &evo_params, const Constraint constraint) {
   std::vector<Variables> population(g_population_size);
   std::generate(population.begin(), population.end(), [&rand, &constraint]() {
      return Variables{
              rand.next_int(constraint.moastery_score.min, constraint.moastery_score.max),
              rand.next_int(constraint.grass_penalty.min, constraint.grass_penalty.max),
              rand.next_u8(constraint.min_figure_count.min, constraint.min_figure_count.max),
              rand.next_int(constraint.grass_score.min, constraint.grass_score.max),
              rand.next_int(constraint.tile_type_score.min, constraint.tile_type_score.max),
              rand.next_int(constraint.tile_close_score.min, constraint.tile_close_score.max),
              rand.next_int(constraint.tile_open_score.min, constraint.tile_open_score.max),
              rand.next_int(constraint.ignore_figure_score_treshold.min, constraint.ignore_figure_score_treshold.max),
      };
   });

   std::vector<double> fitness(g_population_size);
   std::transform(population.begin(), population.end(), fitness.begin(), objective_function);

   auto it_optimum = std::min(fitness.begin(), fitness.end());

   for (std::size_t i = 0; i < g_iteration_count; ++i) {
      for (std::size_t p = 0; p < g_population_size; ++p) {
         auto chosen = util::choose_n<Variables, 3>(rand, population);
         Variables mutant{
            mutate<int>(constraint.moastery_score.min, constraint.moastery_score.max, chosen[0].moastery_score, chosen[1].moastery_score, chosen[2].moastery_score),
            mutate<int>(constraint.grass_penalty.min, constraint.grass_penalty.max, chosen[0].grass_penalty, chosen[1].grass_penalty, chosen[2].grass_penalty),
            mutate<int>(constraint.min_figure_count.min, constraint.min_figure_count.max, chosen[0].min_figure_count, chosen[1].min_figure_count, chosen[2].min_figure_count),
            mutate<int>(constraint.grass_score.min, constraint.grass_score.max, chosen[0].grass_score, chosen[1].grass_score, chosen[2].grass_score),
            mutate<int>(constraint.tile_type_score.min, constraint.tile_type_score.max, chosen[0].tile_type_score, chosen[1].tile_type_score, chosen[2].tile_type_score),
            mutate<int>(constraint.tile_close_score.min, constraint.tile_close_score.max, chosen[0].tile_close_score, chosen[1].tile_close_score, chosen[2].tile_close_score),
            mutate<int>(constraint.tile_open_score.min, constraint.tile_open_score.max, chosen[0].tile_open_score, chosen[1].tile_open_score, chosen[2].tile_open_score),
            mutate<int>(constraint.ignore_figure_score_treshold.min, constraint.ignore_figure_score_treshold.max, chosen[0].ignore_figure_score_treshold, chosen[1].ignore_figure_score_treshold, chosen[2].ignore_figure_score_treshold),
         };

         if (rand.next_double(1.0) < g_cross) {
            mutant.moastery_score = population[p].moastery_score;
         }
         if (rand.next_double(1.0) < g_cross) {
            mutant.grass_penalty = population[p].grass_penalty;
         }
         if (rand.next_double(1.0) < g_cross) {
            mutant.min_figure_count = population[p].min_figure_count;
         }
         if (rand.next_double(1.0) < g_cross) {
            mutant.grass_score = population[p].grass_score;
         }
         if (rand.next_double(1.0) < g_cross) {
            mutant.tile_type_score = population[p].tile_type_score;
         }
         if (rand.next_double(1.0) < g_cross) {
            mutant.tile_close_score = population[p].tile_close_score;
         }
         if (rand.next_double(1.0) < g_cross) {
            mutant.tile_open_score = population[p].tile_open_score;
         }
         if (rand.next_double(1.0) < g_cross) {
            mutant.ignore_figure_score_treshold = population[p].ignore_figure_score_treshold;
         }

         auto mutant_fitness = objective_function(mutant);

         if (mutant_fitness < fitness[p]) {
            fitness[p] = mutant_fitness;
            population[p] = mutant;
            if (mutant_fitness < *it_optimum) {
               it_optimum = fitness.begin() + p;
            }
         }
      }
   }

   return population[it_optimum - fitness.begin()];
}


}// namespace evolution