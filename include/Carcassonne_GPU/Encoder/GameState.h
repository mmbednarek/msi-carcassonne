#ifndef CARCASSONNE_GPU_GAMESTATE_H
#define CARCASSONNE_GPU_GAMESTATE_H
#ifdef __CUDACC__
#define DEVHOST __device__ __host__
#else
#define DEVHOST
#endif

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#include <Carcassonne_GPU/Game/Game.h>
#include <Util_GPU/mb/result.h>
#include "IBoolWriter.h"

namespace carcassonne::encoder {

bool encode_game_state(const game::Game &game, IBoolWriter &writer);

}

#endif//CARCASSONNE_GPU_GAMESTATE_H
