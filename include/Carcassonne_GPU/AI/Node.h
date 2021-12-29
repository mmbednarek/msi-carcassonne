#ifndef CARCASSONNE_GPU_NODE_H
#define CARCASSONNE_GPU_NODE_H
#ifdef __CUDACC__
#define DEVHOST __device__ __host__
#else
#define DEVHOST
#endif

#include "AI.h"
#include <Carcassonne_GPU/Player.h>
#include <Carcassonne_GPU/Game/Move.h>
#include <Carcassonne_GPU/Game/Game.h>
#include <functional>
#include <mb/int.h>
#include <memory>

// Node represents the state resulted by choosing an action

namespace carcassonne::ai {

constexpr double g_C = 0.3;

class Tree;

class Node {
   game::Game m_game;
   Player m_player;
   NodeId m_parent_id{};
   std::vector<NodeId> m_children;
   NodeId m_id = 0;
   game::FullMove m_move;
   bool m_expanded = false;

 public:
   mb::size m_simulation_count = 0;
   std::array<mb::size, 4> m_player_wins{};

   Node(game::Game &&game, const Player &player, game::FullMove move);
   Node(NodeId id, game::Game &&game, const Player &player, game::FullMove move, NodeId parent_id);

   Node(Node &&) noexcept = default;
   Node &operator=(Node &&) noexcept = default;
   Node(const Node &other) = delete;
   Node &operator=(const Node &other) = delete;

   Player find_winner();

   [[nodiscard]] DEVHOST inline Player find_winning_player() const noexcept {
      auto win_it = std::find_if(m_player_wins.begin(), m_player_wins.end(), [](mb::size wins) { return wins != 0; });
      if (win_it == m_player_wins.end())
         return Player::Black;
      return static_cast<Player>(*win_it);
   }

   DEVHOST constexpr void update_id(NodeId id) noexcept {
      m_id = id;
   }

   DEVHOST constexpr void update_parent_id(NodeId id) noexcept {
      m_parent_id = id;
   }

   DEVHOST inline void clear_children() noexcept {
      m_children.clear();
   }

   [[nodiscard]] DEVHOST constexpr mb::size player_wins(Player player) const noexcept {
      return m_player_wins[static_cast<mb::size>(player)];
   }

   [[nodiscard]] double UCT1(unsigned long rollout_count) const noexcept;

   [[nodiscard]] game::Game &game() const noexcept;

   [[nodiscard]] DEVHOST constexpr mb::u64 id() const noexcept {
      return m_id;
   }
   [[nodiscard]] DEVHOST constexpr const Player &player() const noexcept {
      return m_player;
   }
   [[nodiscard]] DEVHOST constexpr NodeId parent_id() const noexcept {
      return m_parent_id;
   }
   [[nodiscard]] DEVHOST constexpr std::vector<NodeId> &children() noexcept {
      return m_children;
   }
   [[nodiscard]] DEVHOST constexpr game::FullMove move() const noexcept {
      return m_move;
   }
   [[nodiscard]] DEVHOST constexpr bool expanded() const noexcept {
      return m_expanded;
   }

   [[nodiscard]] DEVHOST constexpr double win_ratio() const noexcept {
      if (m_simulation_count == 0) {
         return 0.5;
      }
      return static_cast<double>(player_wins(m_player)) / static_cast<double>(m_simulation_count);
   }

   [[nodiscard]] DEVHOST constexpr mb::size simulation_count() const noexcept {
      return m_simulation_count;
   }

   DEVHOST constexpr void propagate(Player player) noexcept {
      m_player_wins[static_cast<mb::size>(player)] += 1;
      m_simulation_count += 1;
   }

   DEVHOST inline void mark_as_expanded() noexcept {
      m_expanded = true;
      m_game = nullptr;
   }

   void add_child(NodeId id) noexcept;
};

}// namespace carcassonne::ai

#endif//CARCASSONNE_GPU_NODE_H
