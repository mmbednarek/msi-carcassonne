#ifndef MSI_CARCASSONNE_DECODER_H
#define MSI_CARCASSONNE_DECODER_H
#include <Carcassonne/Move.h>
#include <mb/result.h>
#include <span>

namespace carcassonne::decoder {

float fill_allowed_vec(const IGame *game, std::vector<bool> &allowed_moves, const std::span<float> blob);
FullMove decode_move(const std::unique_ptr<IGame> &game, std::vector<bool> &allowed_moves, const std::span<float> blob, float prob);

}

#endif//MSI_CARCASSONNE_DECODER_H
