#ifndef MSI_CARCASSONNE_DataWithPromise_H
#define MSI_CARCASSONNE_DataWithPromise_H
#include <future>

namespace util {

template <typename T_IN, typename T_OUT>
struct DataWithPromise {
   std::promise<T_OUT> *promise{nullptr};
   T_IN *node{nullptr};

 public:
   DataWithPromise() = default;
   DataWithPromise(std::promise<T_OUT> *_promise, T_IN *_node) : promise(_promise), node(_node) {}
};

} // namespace util

#endif // MSI_CARCASSONNE_DataWithPromise_H
