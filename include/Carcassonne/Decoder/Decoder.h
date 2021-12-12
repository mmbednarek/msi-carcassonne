#ifndef MSI_CARCASSONNE_DECODER_H
#define MSI_CARCASSONNE_DECODER_H
#include <Carcassonne/Move.h>
#include <mb/result.h>
#include <span>

namespace carcassonne::decoder {

FullMove decode_move(const IGame &game, TileType tile, std::vector<bool> &allowed_moves, std::span<float> blob, float prob);

}

#endif//MSI_CARCASSONNE_DECODER_H
