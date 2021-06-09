#ifndef MSI_CARCASSONNE_PARAMETERS_H
#define MSI_CARCASSONNE_PARAMETERS_H
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
};

}// namespace carcassonne

#endif//MSI_CARCASSONNE_PARAMETERS_H
