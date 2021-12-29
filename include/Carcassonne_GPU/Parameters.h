#ifndef CARCASSONNE_GPU_PARAMETERS_H
#define CARCASSONNE_GPU_PARAMETERS_H
#ifdef __CUDACC__
#define DEVHOST __device__ __host__
#else
#define DEVHOST
#endif

#include <mb/int.h>

namespace carcassonne {

struct Parameters {
   // 7560,10499,2,1506608,8415,6819,7877
   int monastery_score = 7560;
   int grass_penalty = 10499;
   int min_figure_count = 2;
   int grass_score = 1506608;
   int tile_type_score = 8415;
   int tile_close_score = 6819;
   int tile_open_score = 7877;
   int ignore_figure_score_threshold = 1000;
   
   Parameters() = default;
   // Parameters(const Parameters &) = default;
   // Parameters(const evolution::Variables vars)
   //  : monastery_score(vars.monastery_score)
   //  , grass_penalty(vars.grass_penalty)
   //  , min_figure_count(vars.min_figure_count)
   //  , grass_score(vars.grass_score)
   //  , tile_type_score(vars.tile_type_score)
   //  , tile_close_score(vars.tile_close_score)
   //  , tile_open_score(vars.tile_open_score)
   //  , ignore_figure_score_threshold(vars.ignore_figure_score_threshold) {}
};

}// namespace carcassonne

#endif//CARCASSONNE_GPU_PARAMETERS_H
