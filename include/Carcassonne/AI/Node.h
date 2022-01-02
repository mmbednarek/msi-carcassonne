#ifndef MSI_CARCASSONNE_NODE_H
#define MSI_CARCASSONNE_NODE_H
#include "AI.h"
#include <Carcassonne/IGame.h>
#include <Carcassonne/Move.h>
#include <Carcassonne/Player.h>
#include <functional>
#include <mb/int.h>
#include <memory>
#include <mutex>

// Node represents the state resulted by choosing an action

namespace carcassonne::ai {

constexpr double g_C = 0.3;

class Tree;

class Node {
   std::unique_ptr<IGame> m_game;
   std::unique_ptr<std::mutex> m_mutex;
   Player m_player;
   NodePtr m_parent = nullptr;
   std::vector<std::unique_ptr<Node>> m_children;
   FullMove m_move;
   bool m_expanded = false;
   bool m_simulated = false;
   std::mutex m_node_mutex

 public:
   mb::size m_simulation_count = 0;        // (N in the paper)
   std::array<mb::size, 4> m_player_wins{};// (not used in DeepRL)
   std::array<int, 4> m_W{};               // total action values (for DeepRL)
   std::array<float, 4> m_Q{};             // mean action values (for DeepRL)
   float m_P = 0.0;                        // prior probability of selecting the Node (for DeepRL)

   Node(std::unique_ptr<IGame> &&game, const Player &player, FullMove move);
   Node(std::unique_ptr<IGame> &&game, const Player &player, FullMove move, float P, NodePtr parent_ptr);

   Node(Node &&) noexcept = default;
   Node &operator=(Node &&) noexcept = default;
   Node(const Node &other) = delete;
   Node &operator=(const Node &other) = delete;

   Player find_winner();

   [[nodiscard]] inline Player find_winning_player() const noexcept {
      auto win_it = std::find_if(m_player_wins.begin(), m_player_wins.end(), [](mb::size wins) { return wins != 0; });
      if (win_it == m_player_wins.end())
         return Player::Black;
      return static_cast<Player>(*win_it);
   }

   constexpr void update_parent_id(NodePtr id) noexcept {
      m_parent = id;
   }

   inline void clear_children() noexcept {
      m_children.clear();
   }

   [[nodiscard]] constexpr mb::size player_wins(Player player) const noexcept {
      return m_player_wins[static_cast<mb::size>(player)];
   }

   [[nodiscard]] double UCT1(unsigned long rollout_count) const noexcept;

   [[nodiscard]] double PUCT() const noexcept;

   [[nodiscard]] IGame &game() const noexcept;

   [[nodiscard]] constexpr const Player &player() const noexcept {
      return m_player;
   }
   [[nodiscard]] constexpr NodePtr parent() const noexcept {
      return m_parent;
   }
   [[nodiscard]] constexpr std::vector<std::unique_ptr<Node>> &children() noexcept {
      return m_children;
   }
   [[nodiscard]] constexpr FullMove move() const noexcept {
      return m_move;
   }

   [[nodiscard]] constexpr bool expanded() const noexcept {
      return m_expanded;
   }
   [[nodiscard]] constexpr bool simulated() const noexcept {
      return m_simulated;
   }

   [[nodiscard]] constexpr bool is_root() const noexcept {
      return m_parent == nullptr;
   }

   [[nodiscard]] constexpr double win_ratio() const noexcept {
      if (m_simulation_count == 0) {
         return 0.5;
      }
      return static_cast<double>(player_wins(m_player)) / static_cast<double>(m_simulation_count);
   }

   [[nodiscard]] constexpr mb::size simulation_count() const noexcept {
      return m_simulation_count;
   }

   constexpr void propagate(Player winner) noexcept {
      for (mb::size i = 0; i < m_player_wins.size(); ++i) {
         if (static_cast<mb::size>(winner) == i) {
            m_player_wins[i] += 1;
            m_W[i] += 1;
            continue;
         }
         m_W[i] -= 1;
         m_Q[i] = m_W[i] / static_cast<float>(m_simulation_count);
      }
      m_simulation_count += 1;
   }

   inline void propagate_state_value(const float &state_value) noexcept {
      const auto current_player = m_player;
      for (mb::size i = 0; i < m_player_wins.size(); ++i) {
         if (static_cast<mb::size>(current_player) == i) {
            m_W[i] += state_value;
            continue;
         }
         m_W[i] -= state_value;
         m_Q[i] = m_W[i] / static_cast<float>(m_simulation_count);
      }
      m_simulation_count += 1;
   }

   inline void mark_as_expanded() noexcept {
      m_expanded = true;
      m_game = nullptr;
   }


   constexpr void mark_as_root() noexcept {
      m_parent = nullptr;
   }

   NodePtr add_child(std::unique_ptr<IGame> &&game, const Player &player, FullMove move, float P) noexcept;

   [[nodiscard]] std::size_t count_children() const;
};

}// namespace carcassonne::ai

#endif//MSI_CARCASSONNE_NODE_H
