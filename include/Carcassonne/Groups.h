#ifndef MSI_CARCASSONNE_GROUPS_H
#define MSI_CARCASSONNE_GROUPS_H
#include <array>
#include <mb/int.h>
#include <numeric>

namespace carcassonne {

constexpr mb::size g_empty = std::numeric_limits<mb::size>::max();

template<mb::size S>
class Groups {
   std::array<mb::size, S> m_group{};
   std::array<mb::size, S> m_backtrack{};
   std::array<mb::u8, S> m_assignments{};

 public:
   constexpr Groups() {
      std::iota(m_group.begin(), m_group.end(), 0);
      std::fill(m_backtrack.begin(), m_backtrack.end(), g_empty);
   }

   [[nodiscard]] constexpr mb::size group_of(mb::size id) const {
      return m_group[id];
   }

   [[nodiscard]] constexpr bool is_free(mb::size id) const {
      return m_assignments[m_group[id]] == 0;
   }

   constexpr void assign(mb::size id, mb::u8 owner) {
      m_assignments[m_group[id]] |= owner;
   }

   constexpr mb::size join(mb::size a, mb::size b) {
      const auto target_group = m_group[a];
      auto join_id = a;
      while (m_backtrack[join_id] != g_empty)
         join_id = m_backtrack[join_id];

      auto at = m_group[b];
      const auto assigment_b = m_assignments[at];

      if (target_group == at)
         return target_group;

      m_backtrack[join_id] = at;
      while (m_backtrack[at] != g_empty) {
         m_group[at] = target_group;
         at = m_backtrack[at];
      }
      m_group[at] = target_group;
      m_assignments[target_group] |= assigment_b;

      return target_group;
   }
};

}// namespace util

#endif//MSI_CARCASSONNE_GROUPS_H
