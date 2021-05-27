#ifndef MSI_CARCASSONNE_NODE_H
#define MSI_CARCASSONNE_NODE_H
#include <Carcassonne/Game/Game.h>
#include <mb/int.h>
// #include <map>

// Node represents the state resulted by choosing an action

namespace carcassonne::ai {

class Node {
  game::Game m_game;
 public:
  mb::size m_visitation_count = 0;
  mb::size m_profits_sum = 0;

  Node(const game::Game game) : m_game(game) {};
  [[nodiscard]] const game::Game &game() noexcept {
    return m_game;
  }
};

} // namespace carcassonne::game::ai

#endif //MSI_CARCASSONNE_NODE_H
