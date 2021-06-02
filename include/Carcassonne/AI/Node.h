#ifndef MSI_CARCASSONNE_NODE_H
#define MSI_CARCASSONNE_NODE_H
#include <Carcassonne/Game/Game.h>
#include <functional>
#include <mb/int.h>

// Node represents the state resulted by choosing an action

namespace carcassonne::ai {

using NodeId = mb::size;

constexpr double g_C = 1.4142135623730951;

class Node {
   std::unique_ptr<IGame> m_game;
   Player m_player;
   NodeId m_parent_id{};
   std::vector<NodeId> m_children;
   NodeId m_id = 0;
   FullMove m_move;
   bool m_expanded = false;

 public:
   mb::size m_visitation_count = 0;
   mb::size m_wins_count = 0;
   mb::size m_loses_count = 0;
   mb::size m_wins_losses_ratio = 0;

   Node(std::unique_ptr<IGame> game, const Player &player, FullMove move);
   Node(NodeId id, std::unique_ptr<IGame> game, const Player &player, FullMove move, NodeId parent_id);

   Node(Node &&) noexcept = default;
   Node &operator=(Node &&) noexcept = default;
   Node(const Node &other) = delete;
   Node &operator=(const Node &other) = delete;

   void simulation() noexcept;

   double UCT1(mb::u64 &rollouts_performed_count) const noexcept;

   [[nodiscard]] inline IGame &game() noexcept {
      return *m_game;
   }
   [[nodiscard]] constexpr mb::u64 id() const noexcept {
      return m_id;
   }
   [[nodiscard]] constexpr const Player &player() noexcept {
      return m_player;
   }
   [[nodiscard]] NodeId parent_id() const noexcept {
      return m_parent_id;
   }
   [[nodiscard]] constexpr std::vector<NodeId> &children() noexcept {
      return m_children;
   }
   [[nodiscard]] constexpr FullMove move() const noexcept {
      return m_move;
   }
   [[nodiscard]] constexpr bool expanded() const noexcept {
      return m_expanded;
   }

   constexpr void mark_as_expanded() noexcept {
      m_expanded = true;
   }

   void add_child(NodeId id) noexcept;
};

}// namespace carcassonne::ai

#endif//MSI_CARCASSONNE_NODE_H
