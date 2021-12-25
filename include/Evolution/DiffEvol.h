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
#include <fmt/os.h>
#include <numeric>

namespace evolution {

template<typename T = int>
struct Range {
   T min, max;
};

struct Parameters {
   std::size_t population_size;
   std::size_t generations_count;
   double cross_chance_initial;
   double cross_chance_final;
   double mutation_rate_initial;
   double mutation_rate_final;
};

struct ParamsRanges {
   Range<> monastery_score;
   Range<> grass_penalty;
   Range<> min_figure_count;
   Range<> grass_score;
   Range<> tile_type_score;
   Range<> tile_close_score;
   Range<> tile_open_score;
};

template <typename T>
constexpr T mutate(std::size_t generation_id, Parameters evo_params, T min, T max, T a, T b, T c) {
   auto initial = evo_params.mutation_rate_initial;
   auto final = evo_params.mutation_rate_final;
   auto N = static_cast<double>(evo_params.generations_count);
   auto i = static_cast<double>(generation_id);
   auto mutation_rate = i/N * final + (1 - i/N) * initial;
   return std::clamp(a + static_cast<T>(mutation_rate * (b - c)), min, max);
}

template<typename TF>
std::tuple<double, Variables> FindOptimal(util::IRandomGenerator &rand, const TF& objective_function, ParamsRanges constraint, ParamsRanges initial_params, Parameters evo_params, int switch_id, fmt::ostream &log_all, fmt::ostream &log_gen) {
   std::vector<Variables> population(evo_params.population_size);
   std::generate(population.begin(), population.end(), [&rand, &initial_params]() {
     return Variables{
             rand.next_int(initial_params.monastery_score.min, initial_params.monastery_score.max),
             rand.next_int(initial_params.grass_penalty.min, initial_params.grass_penalty.max),
             rand.next_u8(initial_params.min_figure_count.min, initial_params.min_figure_count.max),
             rand.next_int(initial_params.grass_score.min, initial_params.grass_score.max),
             rand.next_int(initial_params.tile_type_score.min, initial_params.tile_type_score.max),
             rand.next_int(initial_params.tile_close_score.min, initial_params.tile_close_score.max),
             rand.next_int(initial_params.tile_open_score.min, initial_params.tile_open_score.max),
     };
   });

   std::vector<double> fitness(evo_params.population_size);
   std::vector<std::future<double>> future_fitness(evo_params.population_size);
   std::transform(population.begin(), population.end(), future_fitness.begin(), [&objective_function](const Variables &vars) {
      return std::async(std::launch::async, objective_function, vars);
   });
   std::transform(future_fitness.begin(), future_fitness.end(), fitness.begin(), [](std::future<double> &f) {
      return f.get();
   });
   auto it_optimum = std::min(fitness.begin(), fitness.end());
   Variables population_center{};
   for (std::size_t i = 0; i < evo_params.generations_count; ++i) {
      std::vector<Variables> mutants;
      mutants.reserve(evo_params.population_size);
      for (std::size_t p = 0; p < evo_params.population_size; ++p) {
         auto chosen = util::choose_n<Variables, 3>(rand, population);
         Variables mutant{
                 mutate(i, evo_params, constraint.monastery_score.min, constraint.monastery_score.max, chosen[0].monastery_score, chosen[1].monastery_score, chosen[2].monastery_score),
                 mutate(i, evo_params, constraint.grass_penalty.min, constraint.grass_penalty.max, chosen[0].grass_penalty, chosen[1].grass_penalty, chosen[2].grass_penalty),
                 mutate(i, evo_params, constraint.min_figure_count.min, constraint.min_figure_count.max, chosen[0].min_figure_count, chosen[1].min_figure_count, chosen[2].min_figure_count),
                 mutate(i, evo_params, constraint.grass_score.min, constraint.grass_score.max, chosen[0].grass_score, chosen[1].grass_score, chosen[2].grass_score),
                 mutate(i, evo_params, constraint.tile_type_score.min, constraint.tile_type_score.max, chosen[0].tile_type_score, chosen[1].tile_type_score, chosen[2].tile_type_score),
                 mutate(i, evo_params, constraint.tile_close_score.min, constraint.tile_close_score.max, chosen[0].tile_close_score, chosen[1].tile_close_score, chosen[2].tile_close_score),
                 mutate(i, evo_params, constraint.tile_open_score.min, constraint.tile_open_score.max, chosen[0].tile_open_score, chosen[1].tile_open_score, chosen[2].tile_open_score),
         };

         auto initial = evo_params.cross_chance_initial;
         auto final = evo_params.cross_chance_final;
         auto N = static_cast<double>(evo_params.generations_count);
         auto cross_chance = static_cast<double>(i)/N * final + (1 - static_cast<double>(i)/N) * initial;

         if (rand.next_double(1.0) < cross_chance) {
            mutant.monastery_score = population[p].monastery_score;
         }
         if (rand.next_double(1.0) < cross_chance) {
            mutant.grass_penalty = population[p].grass_penalty;
         }
         if (rand.next_double(1.0) < cross_chance) {
            mutant.min_figure_count = population[p].min_figure_count;
         }
         if (rand.next_double(1.0) < cross_chance) {
            mutant.grass_score = population[p].grass_score;
         }
         if (rand.next_double(1.0) < cross_chance) {
            mutant.tile_type_score = population[p].tile_type_score;
         }
         if (rand.next_double(1.0) < cross_chance) {
            mutant.tile_close_score = population[p].tile_close_score;
         }
         if (rand.next_double(1.0) < cross_chance) {
            mutant.tile_open_score = population[p].tile_open_score;
         }
         mutants.push_back(mutant);
      }
      std::vector<double> mutants_fitness(evo_params.population_size);
      std::transform(mutants.begin(), mutants.end(), future_fitness.begin(), [&objective_function](const Variables &vars) {
         return std::async(std::launch::async, objective_function, vars);
      });
      std::transform(future_fitness.begin(), future_fitness.end(), mutants_fitness.begin(), [](std::future<double> &f) {
         return f.get();
      });
      for (auto it = mutants.begin(); it != mutants.end(); ++it) {
         auto individual_id = it - mutants.begin();
         auto mutant_fitness = mutants_fitness[individual_id];
         if (mutant_fitness > fitness[individual_id]) {
            fitness[individual_id] = mutant_fitness;
            population[individual_id] = *it;
            if (mutant_fitness > *it_optimum) {
               it_optimum = fitness.begin() + individual_id;
            }
         }
      }
      // fmt::print("Generation nr {}:\n", i);
      auto mean_fitness = std::accumulate(fitness.begin(), fitness.end(), 0.0) / static_cast<double>(evo_params.population_size);
//      fmt::print("\nGeneration nr {}: MeanFitness: ", i);
//      fmt::print("{0:.>+#{1}.{2}G}", mean_fitness, 6, 4);
//      fmt::print(", MaxFitness: {}, Fitness: ", *it_optimum);
      for (auto it = population.begin(); it != population.end(); ++it) {
         auto individual_id = it - population.begin();
         auto fit = fitness[it - population.begin()];
         // fmt::print("winner is {}", fit > 0 ? "Blue! :)" : "Black :(");
         // fmt::print("\tFitness: {}, ", fit);
//         fmt::print("{}, ", fit);
         // fmt::print("\tmoastery_score: {},  ", it->monastery_score);
         // fmt::print("\tgrass_penalty: {}, ", it->grass_penalty);
         // fmt::print("\tmin_figure_count: {}, ", it->min_figure_count);
         // fmt::print("\tgrass_score: {}, ", it->grass_score);
         // fmt::print("\ttile_type_score: {}, ", it->tile_type_score);
         // fmt::print("\ttile_open_score: {}, ", it->tile_open_score);
         // fmt::print("\ttile_close_score: {}\n", it->tile_close_score);
         log_all.print("{},", switch_id * evo_params.generations_count * evo_params.population_size + i * evo_params.population_size + individual_id);
         log_all.print("{},", fit);
         log_all.print("{},", it->monastery_score);
         log_all.print("{},", it->grass_penalty);
         log_all.print("{},", it->min_figure_count);
         log_all.print("{},", it->grass_score);
         log_all.print("{},", it->tile_type_score);
         log_all.print("{},", it->tile_open_score);
         log_all.print("{}\n", it->tile_close_score);
      }
      // fmt::print("\n");
      population_center = Variables {
              static_cast<int>(std::accumulate(population.begin(), population.end(), 0, [](int sum, const Variables& individual){ return sum + individual.monastery_score;}) / evo_params.population_size),
              static_cast<int>(std::accumulate(population.begin(), population.end(), 0, [](int sum, const Variables& individual){ return sum + individual.grass_penalty;}) / evo_params.population_size),
              static_cast<int>(std::accumulate(population.begin(), population.end(), 0, [](int sum, const Variables& individual){ return sum + individual.min_figure_count;}) / evo_params.population_size),
              static_cast<int>(std::accumulate(population.begin(), population.end(), 0, [](int sum, const Variables& individual){ return sum + individual.grass_score;}) / evo_params.population_size),
              static_cast<int>(std::accumulate(population.begin(), population.end(), 0, [](int sum, const Variables& individual){ return sum + individual.tile_type_score;}) / evo_params.population_size),
              static_cast<int>(std::accumulate(population.begin(), population.end(), 0, [](int sum, const Variables& individual){ return sum + individual.tile_close_score;}) / evo_params.population_size),
              static_cast<int>(std::accumulate(population.begin(), population.end(), 0, [](int sum, const Variables& individual){ return sum + individual.tile_open_score;}) / evo_params.population_size),
      };
      fmt::print("{},", switch_id * evo_params.generations_count + i);
      fmt::print("{},", mean_fitness);
      fmt::print("{},", *it_optimum);
      fmt::print("{},", population_center.monastery_score);
      fmt::print("{},", population_center.grass_penalty);
      fmt::print("{},", population_center.min_figure_count);
      fmt::print("{},", population_center.grass_score);
      fmt::print("{},", population_center.tile_type_score);
      fmt::print("{},", population_center.tile_open_score);
      fmt::print("{}\n", population_center.tile_close_score);

      log_gen.print("{},", switch_id * evo_params.generations_count + i);
      log_gen.print("{},", mean_fitness);
      log_gen.print("{},", *it_optimum);
      log_gen.print("{},", population_center.monastery_score);
      log_gen.print("{},", population_center.grass_penalty);
      log_gen.print("{},", population_center.min_figure_count);
      log_gen.print("{},", population_center.grass_score);
      log_gen.print("{},", population_center.tile_type_score);
      log_gen.print("{},", population_center.tile_open_score);
      log_gen.print("{}\n", population_center.tile_close_score);
   }

//   fmt::print("\n({}, {})",
//              (std::min_element(population.begin(), population.end(), [](const Variables &lhs, const Variables &rhs) { return lhs.monastery_score < rhs.monastery_score; }))->monastery_score,
//              (std::max_element(population.begin(), population.end(), [](const Variables &lhs, const Variables &rhs) { return lhs.monastery_score < rhs.monastery_score; }))->monastery_score
//   );
//   fmt::print("({}, {})",
//              (std::min_element(population.begin(), population.end(), [](const Variables &lhs, const Variables &rhs) { return lhs.grass_penalty < rhs.grass_penalty; }))->grass_penalty,
//              (std::max_element(population.begin(), population.end(), [](const Variables &lhs, const Variables &rhs) { return lhs.grass_penalty < rhs.grass_penalty; }))->grass_penalty
//   );
//   fmt::print("({}, {})",
//              (std::min_element(population.begin(), population.end(), [](const Variables &lhs, const Variables &rhs) { return lhs.min_figure_count < rhs.min_figure_count; }))->min_figure_count,
//              (std::max_element(population.begin(), population.end(), [](const Variables &lhs, const Variables &rhs) { return lhs.min_figure_count < rhs.min_figure_count; }))->min_figure_count
//   );
//   fmt::print("({}, {})",
//              (std::min_element(population.begin(), population.end(), [](const Variables &lhs, const Variables &rhs) { return lhs.grass_score < rhs.grass_score; }))->grass_score,
//              (std::max_element(population.begin(), population.end(), [](const Variables &lhs, const Variables &rhs) { return lhs.grass_score < rhs.grass_score; }))->grass_score
//   );
//   fmt::print("({}, {})",
//              (std::min_element(population.begin(), population.end(), [](const Variables &lhs, const Variables &rhs) { return lhs.tile_type_score < rhs.tile_type_score; }))->tile_type_score,
//              (std::max_element(population.begin(), population.end(), [](const Variables &lhs, const Variables &rhs) { return lhs.tile_type_score < rhs.tile_type_score; }))->tile_type_score
//   );
//   fmt::print("({}, {})",
//              (std::min_element(population.begin(), population.end(), [](const Variables &lhs, const Variables &rhs) { return lhs.tile_close_score < rhs.tile_close_score; }))->tile_close_score,
//              (std::max_element(population.begin(), population.end(), [](const Variables &lhs, const Variables &rhs) { return lhs.tile_close_score < rhs.tile_close_score; }))->tile_close_score
//   );
//   fmt::print("({}, {})",
//              (std::min_element(population.begin(), population.end(), [](const Variables &lhs, const Variables &rhs) { return lhs.tile_open_score < rhs.tile_open_score; }))->tile_open_score,
//              (std::max_element(population.begin(), population.end(), [](const Variables &lhs, const Variables &rhs) { return lhs.tile_open_score < rhs.tile_open_score; }))->tile_open_score
//   );
//
//   evolution::ParamsRanges optimal_range {
//           {
//                   (std::min_element(population.begin(), population.end(), [](const Variables &lhs, const Variables &rhs) { return lhs.monastery_score < rhs.monastery_score; }))->monastery_score,
//                   (std::max_element(population.begin(), population.end(), [](const Variables &lhs, const Variables &rhs) { return lhs.monastery_score < rhs.monastery_score; }))->monastery_score
//           },
//           {
//                   (std::min_element(population.begin(), population.end(), [](const Variables &lhs, const Variables &rhs) { return lhs.grass_penalty < rhs.grass_penalty; }))->grass_penalty,
//                   (std::max_element(population.begin(), population.end(), [](const Variables &lhs, const Variables &rhs) { return lhs.grass_penalty < rhs.grass_penalty; }))->grass_penalty
//           },
//           {
//                   (std::min_element(population.begin(), population.end(), [](const Variables &lhs, const Variables &rhs) { return lhs.min_figure_count < rhs.min_figure_count; }))->min_figure_count,
//                   (std::min_element(population.begin(), population.end(), [](const Variables &lhs, const Variables &rhs) { return lhs.min_figure_count < rhs.min_figure_count; }))->min_figure_count
//           },
//           {
//                   (std::min_element(population.begin(), population.end(), [](const Variables &lhs, const Variables &rhs) { return lhs.grass_score < rhs.grass_score; }))->grass_score,
//                   (std::max_element(population.begin(), population.end(), [](const Variables &lhs, const Variables &rhs) { return lhs.grass_score < rhs.grass_score; }))->grass_score
//           },
//           {
//                   (std::min_element(population.begin(), population.end(), [](const Variables &lhs, const Variables &rhs) { return lhs.tile_type_score < rhs.tile_type_score; }))->tile_type_score,
//                   (std::max_element(population.begin(), population.end(), [](const Variables &lhs, const Variables &rhs) { return lhs.tile_type_score < rhs.tile_type_score; }))->tile_type_score
//           },
//           {
//                   (std::min_element(population.begin(), population.end(), [](const Variables &lhs, const Variables &rhs) { return lhs.tile_close_score < rhs.tile_close_score; }))->tile_close_score,
//                   (std::max_element(population.begin(), population.end(), [](const Variables &lhs, const Variables &rhs) { return lhs.tile_close_score < rhs.tile_close_score; }))->tile_close_score
//           },
//           {
//                   (std::min_element(population.begin(), population.end(), [](const Variables &lhs, const Variables &rhs) { return lhs.tile_open_score < rhs.tile_open_score; }))->tile_open_score,
//                   (std::max_element(population.begin(), population.end(), [](const Variables &lhs, const Variables &rhs) { return lhs.tile_open_score < rhs.tile_open_score; }))->tile_open_score
//           },
//   };
   auto mean_fitness = std::accumulate(fitness.begin(), fitness.end(), 0.0) / static_cast<double>(evo_params.population_size);
   return std::make_tuple(mean_fitness, population_center);
//   return population[it_optimum - fitness.begin()];
}

}// namespace evolution

#endif//MSI_DIFFEVOL_H
