#ifndef MSI_CARCASSONNE_NODE_H
#define MSI_CARCASSONNE_NODE_H
#include "AI.h"
#include <Carcassonne/Player.h>
#include <Carcassonne/Move.h>
#include <Carcassonne/IGame.h>
#include <functional>
#include <mb/int.h>
#include <memory>

// Node represents the state resulted by choosing an action

namespace carcassonne::ai {

//constexpr double g_C = 1.4142135623730951;
constexpr double g_C = 0;

class Tree;

class Node {
   std::unique_ptr<IGame> m_game;
   Player m_player;
   NodeId m_parent_id{};
   std::vector<NodeId> m_children;
   NodeId m_id = 0;
   FullMove m_move;
   bool m_expanded = false;

 public:
   mb::size m_simulation_count = 0;
   std::array<mb::size, 4> m_player_wins{};

   Node(std::unique_ptr<IGame> &&game, const Player &player, FullMove move);
   Node(NodeId id, std::unique_ptr<IGame> &&game, const Player &player, FullMove move, NodeId parent_id);

   Node(Node &&) noexcept = default;
   Node &operator=(Node &&) noexcept = default;
   Node(const Node &other) = delete;
   Node &operator=(const Node &other) = delete;

   void mark_as_leaf_node();

   constexpr void update_id(NodeId id) noexcept {
      m_id = id;
   }

   constexpr void update_parent_id(NodeId id) noexcept {
      m_parent_id = id;
   }

   inline void clear_children() noexcept {
      m_children.clear();
   }

   [[nodiscard]] constexpr mb::size player_wins(Player player) const noexcept {
      return m_player_wins[static_cast<mb::size>(player)];
   }

   [[nodiscard]] double UCT1(unsigned long rollout_count) const noexcept;

   [[nodiscard]] IGame &game() const noexcept;

   [[nodiscard]] constexpr mb::u64 id() const noexcept {
      return m_id;
   }
   [[nodiscard]] constexpr const Player &player() const noexcept {
      return m_player;
   }
   [[nodiscard]] constexpr NodeId parent_id() const noexcept {
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

   [[nodiscard]] constexpr double win_ratio() const noexcept {
      const auto player_wins = m_player_wins[static_cast<int>(m_player)];
      if (player_wins == m_simulation_count) {
         return static_cast<double>(player_wins);
      }
      return static_cast<double>(player_wins) / static_cast<double>(m_simulation_count);
   }

   [[nodiscard]] constexpr mb::size simulation_count() const noexcept {
      return m_simulation_count;
   }

   constexpr void propagate(Node &child, mb::size player_count) noexcept {
      for (auto i = 0; i < player_count; ++i) {
         m_player_wins[i] += child.m_player_wins[i];
      }
      m_simulation_count += child.m_simulation_count;
   }

   inline void mark_as_expanded() noexcept {
      m_expanded = true;
      m_game = nullptr;
   }

   void add_child(NodeId id) noexcept;
};

}// namespace carcassonne::ai

#endif//MSI_CARCASSONNE_NODE_H
