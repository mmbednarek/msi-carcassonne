#ifndef CARCASSONNE_GPU_DECODER_H
#define CARCASSONNE_GPU_DECODER_H
#ifdef __CUDACC__
#define DEVHOST __device__ __host__
#else
#define DEVHOST
#endif

#include <Carcassonne_GPU/Game/Move.h>
#include <Carcassonne_GPU/Game/Game.h>
#include <Util/span_cpp17.h>

namespace carcassonne::decoder {

game::FullMove decode_move(const game::Game &game, TileType tile, std::vector<bool> &allowed_moves, my_std::span<float> blob, float prob);

}

#endif//CARCASSONNE_GPU_DECODER_H
