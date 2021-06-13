#ifndef MSI_DIFFEVOL_H
#define MSI_DIFFEVOL_H
#include <Util/IRandomGenerator.h>
#include <mb/int.h>
#include <random>
#include <functional>
#include <cmath>
#include <future>
#include <algorithm>
#include <fmt/core.h>

namespace evolution {

constexpr auto g_population_size = 50;
constexpr auto g_iteration_count = 1000;
constexpr auto g_mutation = 0.8;
constexpr auto g_cross = 0.2;

template<typename T = int>
struct Range {
   T min, max;
};

struct Variables {
   int monastery_score = 2000;
   int grass_penalty = 20;
   int min_figure_count = 2;
   int grass_score = 4;
   int tile_type_score = 4;
   int tile_close_score = 20000;
   int tile_open_score = 200;
   int ignore_figure_score_threshold = 900;
};

struct Parameters {
   std::size_t population_size;
   std::size_t generations_count;
   double mutation_chance;
   double cross_chance;
   double mutation_rate_initial;
   double mutation_rate_final;
   double optimal_fitness;
   // std::size_t polynomial_degree;
};

struct Constraint {
   Range<> monastery_score;
   Range<> grass_penalty;
   Range<> min_figure_count;
   Range<> grass_score;
   Range<> tile_type_score;
   Range<> tile_close_score;
   Range<> tile_open_score;
   Range<> ignore_figure_score_threshold;
};

template <typename T>
constexpr T mutate(T min, T max, T a, T b, T c) {
   return std::clamp(a + static_cast<T>(g_mutation * (b - c)), min, max);
}

template<typename TF>
Variables FindOptimal(util::IRandomGenerator &rand, const TF& objective_function, evolution::Parameters &evo_params, Constraint constraint) {
   std::vector<Variables> population(g_population_size);
   std::generate(population.begin(), population.end(), [&rand, &constraint]() {
     return Variables{
             rand.next_int(constraint.monastery_score.min, constraint.monastery_score.max),
             rand.next_int(constraint.grass_penalty.min, constraint.grass_penalty.max),
             rand.next_u8(constraint.min_figure_count.min, constraint.min_figure_count.max),
             rand.next_int(constraint.grass_score.min, constraint.grass_score.max),
             rand.next_int(constraint.tile_type_score.min, constraint.tile_type_score.max),
             rand.next_int(constraint.tile_close_score.min, constraint.tile_close_score.max),
             rand.next_int(constraint.tile_open_score.min, constraint.tile_open_score.max),
             rand.next_int(constraint.ignore_figure_score_threshold.min, constraint.ignore_figure_score_threshold.max),
     };
   });

   std::vector<double> fitness(g_population_size);






   // std::vector<std::pair<double, Variables>> fits_vars(g_population_size);
   // std::vector<std::vector<std::pair<double, Variables>>> generations(evo_params.generations_count);
   // std::vector<double> fitness(g_population_size);
   // // std::vector<double> fitness_to_normalise(g_population_size);
   // // double fitness_to_normalise_sum{};
   // std::vector<std::pair<double, Variables>> normalised_fits(g_population_size);
   // fmt::print("\nEVALUATING {} HEURISTIC PLAYERS\n", g_population_size);


   // std::vector<std::future<double>> future_fitness(g_population_size);
   // std::transform(population.begin(), population.end(), future_fitness.begin(), [&objective_function](const Variables &vars) {
   //    return std::async(std::launch::async, objective_function, vars);
   // });
   // std::transform(future_fitness.begin(), future_fitness.end(), fitness.begin(), [&ant_nr](std::future<double> &f) {
   //    ant_nr++;
   //    fmt::print("ai{}, ", ant_nr);
   //    return f.get();
   // });

   // auto it_optimum = std::min(fitness.begin(), fitness.end());
   // auto it_optimum_population = generations.begin();
   // std::transform(fitness.begin(), fitness.end(), population.begin(), fits_vars.begin(), [](double fit, Variables vars) {
   //    return std::make_pair(fit, vars);
   // });





   std::transform(population.begin(), population.end(), fitness.begin(), objective_function);

   auto it_optimum = std::min(fitness.begin(), fitness.end());

   for (std::size_t i = 0; i < g_iteration_count; ++i) {
      // std::vector<Variables> mutants;
      // mutants.reserve(g_population_size);
      for (std::size_t p = 0; p < g_population_size; ++p) {
         auto chosen = util::choose_n<Variables, 3>(rand, population);
         Variables mutant{
                 mutate(constraint.monastery_score.min, constraint.monastery_score.max, chosen[0].monastery_score, chosen[1].monastery_score, chosen[2].monastery_score),
                 mutate(constraint.grass_penalty.min, constraint.grass_penalty.max, chosen[0].grass_penalty, chosen[1].grass_penalty, chosen[2].grass_penalty),
                 mutate(constraint.min_figure_count.min, constraint.min_figure_count.max, chosen[0].min_figure_count, chosen[1].min_figure_count, chosen[2].min_figure_count),
                 mutate(constraint.grass_score.min, constraint.grass_score.max, chosen[0].grass_score, chosen[1].grass_score, chosen[2].grass_score),
                 mutate(constraint.tile_type_score.min, constraint.tile_type_score.max, chosen[0].tile_type_score, chosen[1].tile_type_score, chosen[2].tile_type_score),
                 mutate(constraint.tile_close_score.min, constraint.tile_close_score.max, chosen[0].tile_close_score, chosen[1].tile_close_score, chosen[2].tile_close_score),
                 mutate(constraint.tile_open_score.min, constraint.tile_open_score.max, chosen[0].tile_open_score, chosen[1].tile_open_score, chosen[2].tile_open_score),
                 mutate(constraint.ignore_figure_score_threshold.min, constraint.ignore_figure_score_threshold.max, chosen[0].ignore_figure_score_threshold, chosen[1].ignore_figure_score_threshold, chosen[2].ignore_figure_score_threshold),
         };

         if (rand.next_double(1.0) < g_cross) {
            mutant.monastery_score = population[p].monastery_score;
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
            mutant.ignore_figure_score_threshold = population[p].ignore_figure_score_threshold;
         }
         // mutants.at(p) = mutant;
         auto mutant_fitness = objective_function(mutant);


      // fmt::print("\n\nEWALUATING {}/{} GENERATION\n", i, evo_params.generations_count);
      // std::transform(population.begin(), population.end(), future_fitness.begin(), [&objective_function](const Variables &vars) {
      //    return std::async(std::launch::async, objective_function, vars);
      // });
      // std::transform(future_fitness.begin(), future_fitness.end(), fitness.begin(), [&ant_nr](std::future<double> &f) {
      //    ant_nr++;
      //    fmt::print("ant{}, ", ant_nr);
      //    return f.get();
      // });
      // std::transform(fitness.begin(), fitness.end(), population.begin(), fits_vars.begin(), [](double fit, Variables vars) {
      //    return std::make_pair(fit, vars);
      // });
      // auto it_optimum = std::min(fitness.begin(), fitness.end());
      // optimal_over_generations[i] = population[it_optimum - fitness.begin()];
      // optimal_fitness[i] = *it_optimum;
      // fmt::print("\nEVALUATION FINISHED\n\n");

      // for (const auto &fw : fits_vars) {
         if (mutant_fitness > fitness[p]) {
            fitness[p] = mutant_fitness;
            population[p] = mutant;
            if (mutant_fitness > *it_optimum) {
               it_optimum = fitness.begin() + p;
            }
         }
      // }
      }
      fmt::print("\nGeneration nr {}:\n", i);
      for (auto it = population.begin(); it != population.end(); it++) {
         auto fit = fitness[it - population.begin()];
         fmt::print("winner is {}", fit > 0 ? "Blue! :)" : "Black :(");
         fmt::print("\tFitness: {}, ", fit);
         fmt::print("\tmoastery_score: {},  ", it->monastery_score);
         fmt::print("\tgrass_penalty: {}, ", it->grass_penalty);
         fmt::print("\tmin_figure_count: {}, ", it->min_figure_count);
         fmt::print("\tgrass_score: {}, ", it->grass_score);
         fmt::print("\ttile_type_score: {}, ", it->tile_type_score);
         fmt::print("\ttile_open_score: {}, ", it->tile_open_score);
         fmt::print("\tignore_figure_score_treshold: {}, ", it->ignore_figure_score_threshold);
         fmt::print("\ttile_close_score: {}\n", it->tile_close_score);
      }
      fmt::print("\n");
   }

   return population[it_optimum - fitness.begin()];
}

}// namespace evolution

#endif//MSI_DIFFEVOL_H
