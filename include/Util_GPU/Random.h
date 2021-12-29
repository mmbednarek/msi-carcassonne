#ifndef CARCASSONNE_RANDOM_H
#define CARCASSONNE_RANDOM_H
#ifdef __CUDACC__
#define DEVHOST __device__ __host__
#else
#define DEVHOST
#endif

namespace util_GPU {

template<typename TI>
[[nodiscard]] constexpr auto random_from_range(auto &generator, const TI &from, const TI &to) -> TI {
   std::uniform_int_distribution<typename TI::difference_type> distribution(0, to - from);
   return from + distribution(generator);
}

}//namespace util_GPU

#endif//CARCASSONNE_RANDOM_H
