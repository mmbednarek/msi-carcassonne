#ifndef CARCASSONNE_POSITION_H
#define CARCASSONNE_POSITION_H
#ifdef __CUDACC__
#define DEVHOST __device__ __host__
#else
#define DEVHOST
#endif

namespace carcassonne {

struct TilePosition {
   int x, y;
};

}

#endif//CARCASSONNE_POSITION_H
