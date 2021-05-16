#ifndef MSI_CARCASSONNE_GROUPS_H
#define MSI_CARCASSONNE_GROUPS_H
#include <Carcassonne/Core.h>
#include <array>
#include <mb/int.h>
#include <numeric>

namespace carcassonne {

using Group = mb::size;

constexpr mb::size g_empty = std::numeric_limits<mb::size>::max();

template<mb::size S>
class Groups {
   std::array<Group, S> m_group{};
   std::array<Group, S> m_backtrack{};
   std::array<PlayerAssignment, S> m_assignments{};
   std::array<EdgeType, S> m_types{};
   std::array<mb::u8, S> m_free_edges{};
   std::array<mb::u8, S> m_tile_count{};

 public:
   constexpr Groups() {
      std::iota(m_group.begin(), m_group.end(), 0);
      std::fill(m_backtrack.begin(), m_backtrack.end(), g_empty);
   }

   [[nodiscard]] constexpr Group group_of(Edge id) const {
      return m_group[id];
   }

   [[nodiscard]] constexpr bool is_free(Edge id) const {
      return static_cast<mb::u8>(m_assignments[m_group[id]]) == 0;
   }

   [[nodiscard]] constexpr EdgeType type_of(Edge id) const {
      return m_types[m_group[id]];
   }

   [[nodiscard]] constexpr bool is_completed(Edge id) const {
      return m_free_edges[m_group[id]] == 0;
   }

   [[nodiscard]] constexpr mb::u8 free_edges(Edge id) const {
      return m_free_edges[m_group[id]];
   }

   [[nodiscard]] constexpr mb::u8 tile_count(Edge id) const {
      return m_tile_count[m_group[id]];
   }

   [[nodiscard]] constexpr PlayerAssignment assigment(Edge id) const {
      return m_assignments[m_group[id]];
   }

   constexpr void set_type(Edge id, EdgeType tt) {
      m_types[m_group[id]] = tt;
   }

   constexpr void assign(Edge id, Player owner) {
      m_assignments[m_group[id]] |= owner;
   }

   constexpr void inc_free_edges(Edge id) {
      ++m_free_edges[m_group[id]];
   }

   constexpr void dec_free_edges(Edge id) {
      --m_free_edges[m_group[id]];
   }

   constexpr void inc_tiles(Edge id) {
      ++m_tile_count[m_group[id]];
   }

   constexpr Group join(Edge a, Edge b) {
      const auto target_group = m_group[a];
      auto join_id = a;
      while (m_backtrack[join_id] != g_empty)
         join_id = m_backtrack[join_id];

      auto at = m_group[b];
      const auto assigment_b = m_assignments[at];
      const auto free_edges_b = m_free_edges[at];
      const auto tile_count_b = m_tile_count[at];

      if (target_group == at)
         return target_group;

      m_backtrack[join_id] = at;
      while (m_backtrack[at] != g_empty) {
         m_group[at] = target_group;
         at = m_backtrack[at];
      }
      m_group[at] = target_group;

      if ((m_assignments[target_group] & assigment_b) != PlayerAssignment::None) {
         m_assignments[target_group] &= assigment_b;
      } else {
         m_assignments[target_group] |= assigment_b;
      }

      m_free_edges[target_group] += free_edges_b;
      m_tile_count[target_group] += tile_count_b;

      return target_group;
   }
};

}// namespace carcassonne

#endif//MSI_CARCASSONNE_GROUPS_H
