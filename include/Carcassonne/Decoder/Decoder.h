#ifndef MSI_CARCASSONNE_DECODER_H
#define MSI_CARCASSONNE_DECODER_H
#include <Carcassonne/Move.h>
#include <mb/result.h>
#include <thrust/host_vector.h>

namespace carcassonne::decoder {

FullMove decode_move(const std::unique_ptr<IGame> &game, std::vector<bool> &allowed_moves, thrust::host_vector<float>& blob, float prob);

}

#endif//MSI_CARCASSONNE_DECODER_H
