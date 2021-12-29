#ifndef CARCASSONNE_GPU_IBOOLWRITER_H
#define CARCASSONNE_GPU_IBOOLWRITER_H
#ifdef __CUDACC__
#define DEVHOST __device__ __host__
#else
#define DEVHOST
#endif

namespace carcassonne::encoder {

class IBoolWriter {
 public:
   virtual void write_bool(bool value) = 0;
};

}

#endif//CARCASSONNE_GPU_IBOOLWRITER_H
