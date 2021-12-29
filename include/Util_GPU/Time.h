#ifndef CARCASSONNE_TIME_H
#define CARCASSONNE_TIME_H
#ifdef __CUDACC__
#define DEVHOST __device__ __host__
#else
#define DEVHOST
#endif
#include <chrono>

namespace util_GPU {

inline uint64_t unix_time() {
   return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

}//namespace util_GPU

#endif//CARCASSONNE_TIME_H
