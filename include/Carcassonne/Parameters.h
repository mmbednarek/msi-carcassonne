#ifndef MSI_CARCASSONNE_PARAMETERS_H
#define MSI_CARCASSONNE_PARAMETERS_H
#include <Evolution/DiffEvol.h>
#include <mb/int.h>

namespace carcassonne {

struct Parameters {
   int monastery_score = 3090;
   int grass_penalty = 41;
   int min_figure_count = 2;
   int grass_score = 79;
   int tile_type_score = 1;
   int tile_close_score = 430;
   int tile_open_score = 740;
   int ignore_figure_score_threshold = 619;
   
   Parameters() = default;
   // Parameters(const Parameters &) = default;
   Parameters& operator=(const evolution::Variables &vars) {
      monastery_score = vars.monastery_score;
      grass_penalty = vars.grass_penalty;
      min_figure_count = vars.min_figure_count;
      grass_score = vars.grass_score;
      tile_type_score = vars.tile_type_score;
      tile_close_score = vars.tile_close_score;
      tile_open_score = vars.tile_open_score;
      ignore_figure_score_threshold = vars.ignore_figure_score_threshold;
      return *this;
   }
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

#endif//MSI_CARCASSONNE_PARAMETERS_H
