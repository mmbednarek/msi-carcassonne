#ifndef MSI_DIFFEVOL_H
#define MSI_DIFFEVOL_H
#include <Util/IRandomGenerator.h>
#include <mb/int.h>
#include <random>
#include <functional>

namespace evolution {

template<typename T = int>
struct Range {
   T min, max;
};

struct Variables {
   int moastery_score = 2000;
   int grass_penalty = 20;
   mb::u8 min_figure_count = 2;
   int grass_score = 4;
   int tile_type_score = 4;
   int tile_close_score = 20000;
   int tile_open_score = 200;
   int ignore_figure_score_treshold = 900;
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
   Range<> moastery_score;
   Range<> grass_penalty;
   Range<mb::u8> min_figure_count;
   Range<> grass_score;
   Range<> tile_type_score;
   Range<> tile_close_score;
   Range<> tile_open_score;
   Range<> ignore_figure_score_treshold;
};

using ObjectiveFunction = std::function<double(const Variables &)>;

Variables FindOptimal(util::IRandomGenerator &rand, const ObjectiveFunction&objective_function, evolution::Parameters &evo_params, Constraint constraint);

}// namespace evolution

#endif//MSI_DIFFEVOL_H
