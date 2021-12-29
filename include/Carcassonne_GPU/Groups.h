#ifndef CARCASSONNE_GPU_GROUPS_H
#define CARCASSONNE_GPU_GROUPS_H
#ifdef __CUDACC__
#define DEVHOST __device__ __host__
#else
#define DEVHOST
#endif

#include <Carcassonne_GPU/Game/Game.h>
#include <array>
#include <mb/int.h>
#include <numeric>
#include "Edge.h"
#include "Player.h"

namespace carcassonne {

using Group = mb::size;

constexpr mb::size g_empty = std::numeric_limits<mb::size>::max();

template<mb::size S>
class Groups {
   struct Info {
      carcassonne::PlayerAssignment assignment = carcassonne::PlayerAssignment::None;
      EdgeType type{};
      mb::u8 free_edges{}, tile_count{};
   };
   std::unordered_map<Edge, Group> m_group;
   std::unordered_map<Group, Group> m_backtrack;
   std::unordered_map<Group, Info> m_info;

 public:
   [[nodiscard]] inline Group group_of(Edge id) const {
      auto group_it = m_group.find(id);
      if (group_it == m_group.end())
         return static_cast<Group>(id);
      return group_it->second;
   }

   [[nodiscard]] inline Group backtrack(Group id) {
      auto back_it = m_backtrack.find(id);
      if (back_it == m_backtrack.end())
         return g_empty;
      return back_it->second;
   }

   [[nodiscard]] inline Info info_at(Edge id) const {
      auto info_it = m_info.find(group_of(id));
      if (info_it == m_info.end())
         return Info{};
      return info_it->second;
   }

   [[nodiscard]] constexpr bool is_free(Edge id) const {
      return info_at(id).assignment == PlayerAssignment::None;
   }

   [[nodiscard]] constexpr EdgeType type_of(Edge id) const {
      return info_at(id).type;
   }

   [[nodiscard]] constexpr mb::u8 free_edges(Edge id) const {
      return info_at(id).free_edges;
   }

   [[nodiscard]] constexpr bool is_completed(Edge id) const {
      return info_at(id).free_edges == 0;
   }

   [[nodiscard]] constexpr mb::u8 tile_count(Edge id) const {
      return info_at(id).tile_count;
   }

   [[nodiscard]] constexpr PlayerAssignment assigment(Edge id) const {
      return info_at(id).assignment;
   }

   constexpr void set_type(Edge id, EdgeType tt) {
      m_info[group_of(id)].type = tt;
   }

   constexpr void assign(Edge id, carcassonne::Player owner) {
      m_info[group_of(id)].assignment |= owner;
   }

   constexpr void inc_free_edges(Edge id) {
      ++m_info[group_of(id)].free_edges;
   }

   constexpr void dec_free_edges(Edge id) {
      --m_info[group_of(id)].free_edges;
   }

   constexpr void inc_tiles(Edge id) {
      ++m_info[group_of(id)].tile_count;
   }

   constexpr Group join(Edge a, Edge b) {
      const auto target_group = group_of(a);
      auto join_id = a;
      while (backtrack(join_id) != g_empty)
         join_id = backtrack(join_id);

      auto at = group_of(b);
      const auto &info_b = m_info[at];

      if (target_group == at)
         return target_group;

      m_backtrack[join_id] = at;
      while (backtrack(at) != g_empty) {
         m_group[at] = target_group;
         at = backtrack(at);
      }
      m_group[at] = target_group;

      auto &target_info = m_info[target_group];

      if ((target_info.assignment & info_b.assignment) != PlayerAssignment::None) {
         target_info.assignment &= info_b.assignment;
      } else {
         target_info.assignment |= info_b.assignment;
      }

      target_info.free_edges += info_b.free_edges;
      target_info.tile_count += info_b.tile_count;

      return target_group;
   }
};

}// namespace carcassonne

#endif//CARCASSONNE_GPU_GROUPS_H
