#ifndef MSI_CARCASSONNE_RANDOM_H
#define MSI_CARCASSONNE_RANDOM_H
#include <random>
#include <array>
#include <set>

namespace util {

template<typename TI>
[[nodiscard]] constexpr auto random_from_range(auto &generator, const TI &from, const TI &to) -> TI {
   std::uniform_int_distribution<typename TI::difference_type> distribution(0, to - from);
   return from + distribution(generator);
}

template<typename TI, typename T>
[[nodiscard]] inline auto random_from_range_picewise(auto &generator, const TI &input, const T &intervals, const T &weights) -> TI {
    std::piecewise_linear_distribution<> distribution(intervals.begin(),intervals.end(),weights.begin());
    return input.begin() + distribution(generator);
}

template<typename T>
[[nodiscard]] inline int next_int_picewise(auto &generator, const T &intervals, const T &weights) {
    std::piecewise_linear_distribution<> distribution(intervals.begin(),intervals.end(),weights.begin());
    return distribution(generator);
}

template<typename T0, std::size_t n, typename T>
std::array<T0, n> choose_n_picewise(auto &generator, const std::vector<T0> &input, const T &intervals, const T &weights) {
   std::array<T0, n> result;
   std::size_t selected_count{};
   std::set<std::size_t> selected;

   while (selected_count != n) {
      std::size_t chosen;
      std::set<std::size_t>::iterator it;
      do {
         chosen = next_int_picewise(generator, intervals, intervals);
         it = selected.find(chosen);
      } while (it != selected.end());

      selected.insert(chosen);
      result[selected_count] = input[chosen];
      ++selected_count;
   }

   return result;
}

}

#endif//MSI_CARCASSONNE_RANDOM_H
