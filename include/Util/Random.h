#ifndef MSI_CARCASSONNE_RANDOM_H
#define MSI_CARCASSONNE_RANDOM_H
#include <random>

namespace util {

template<typename TI>
[[nodiscard]] constexpr auto random_from_range(auto &generator, const TI &from, const TI &to) -> TI {
   std::uniform_int_distribution<typename TI::difference_type> distribution(0, to - from);
   return from + distribution(generator);
}

}

#endif//MSI_CARCASSONNE_RANDOM_H
