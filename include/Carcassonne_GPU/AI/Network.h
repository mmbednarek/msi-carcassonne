#ifndef CARCASSONNE_GPU_NETWORK_H
#define CARCASSONNE_GPU_NETWORK_H
#ifdef __CUDACC__
#define DEVHOST __device__ __host__
#else
#define DEVHOST
#endif

#include <Carcassonne_GPU/Player.h>
#include <Carcassonne_GPU/IGame.h>
#include <mb/int.h>
#include <tuple>

namespace carcassonne::ai {

class Network {
   
};

}// namespace carcassonne::ai

#endif//CARCASSONNE_GPU_NETWORK_H
