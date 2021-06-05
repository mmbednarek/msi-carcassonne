#include <Carcassonne/AI/Node.h>
#include <Carcassonne/ScoreBoard.h>
#include <cmath>
#include <utility>
#include <vector>


namespace carcassonne::ai {

Node::Node(std::unique_ptr<IGame> &&game, const Player &player, FullMove move)
    : m_game(std::move(game)), m_player(player), m_move(move), m_id(0) {}

Node::Node(NodeId id, std::unique_ptr<IGame> &&game, const Player &player, FullMove move, NodeId parent_id)
    : m_game(std::move(game)), m_player(player), m_move(move), m_parent_id(parent_id), m_id(id) {}

double Node::UCT1(mb::size rollout_count) const noexcept {
   const auto player_wins = m_player_wins[static_cast<int>(m_player)];
   if (m_simulation_count == player_wins || rollout_count == 0) {
      return std::numeric_limits<double>::infinity();
   }
   return static_cast<double>(player_wins) / static_cast<double>(m_simulation_count) + g_C * std::sqrt(std::log(static_cast<double>(rollout_count)) / static_cast<double>(m_simulation_count));
}

void Node::add_child(NodeId id) noexcept {
   m_children.push_back(id);
}

void Node::mark_as_leaf_node() {
   auto max_score_it = std::max_element(m_game->scores().begin(), m_game->scores().end(), [](PlayerScore lhs, PlayerScore rhs) {
      return lhs.score < rhs.score;
   });
   m_player_wins[static_cast<int>(max_score_it->player)] = 1;
   m_simulation_count = 1;
   mark_as_expanded();
}

IGame &Node::game() const noexcept {
   return *m_game;
}

}// namespace carcassonne::ai