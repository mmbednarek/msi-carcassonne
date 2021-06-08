#ifndef MSI_CARCASSONNE_PARAMETERS_H
#define MSI_CARCASSONNE_PARAMETERS_H

namespace carcassonne {

struct Parameters {
   int moastery_score = 2000;
   int grass_penalty = 20;
   mb::u8 min_figure_count = 2;
   int grass_score = 4;
   int tile_type_score = 4;
   int tile_close_score = 20000;
   int tile_open_score = 200;
};

}// namespace carcassonne

#endif//MSI_CARCASSONNE_PARAMETERS_H
