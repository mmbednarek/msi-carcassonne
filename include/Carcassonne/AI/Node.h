#ifndef MSI_CARCASSONNE_NODE_H
#define MSI_CARCASSONNE_NODE_H
#include <Carcassonne/Game/Game.h>
#include <mb/int.h>
#include <functional>
// #include <map>

// Node represents the state resulted by choosing an action

namespace carcassonne::ai {

class Node {
  IGame &m_game;
  const Player &m_player;
  std::reference_wrapper<Node> m_parent = std::ref(*this);
  std::vector<std::reference_wrapper<Node>> m_children;
  mb::u64 m_id = 0;
  double g_C = sqrt(2); // TODO: move C to global parameters location
 public:
  mb::size m_visitation_count = 0;
  mb::size m_wins_count = 0;
  mb::size m_loses_count = 0;
  mb::size m_wins_losses_ratio = 0;
  
  Node(IGame &game, const Player &player, mb::u64 &rollouts_performed_count);
  Node(IGame &game, const Player &player, mb::u64 &rollouts_performed_count, std::reference_wrapper<Node> parent);

  [[nodiscard]] std::tuple<std::size_t, std::size_t> simulation() noexcept;

  double UCT1(mb::u64 &rollouts_performed_count) noexcept;

  [[nodiscard]] constexpr IGame &game() noexcept {
    return m_game;
  }
  [[nodiscard]] constexpr mb::u64 &id() noexcept {
    return m_id;
  }
  [[nodiscard]] constexpr const Player &player() noexcept {
    return m_player;
  }
  [[nodiscard]] std::reference_wrapper<Node> &parent() noexcept {
    return m_parent;
  }
  [[nodiscard]] constexpr std::vector<std::reference_wrapper<Node>> &children() noexcept {
    return m_children;
  }
  
};

} // namespace carcassonne::game::ai

#endif //MSI_CARCASSONNE_NODE_H
