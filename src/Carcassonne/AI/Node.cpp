#include <Carcassonne/AI/Node.h>
#include <Carcassonne/ScoreBoard.h>
#include <Carcassonne/Game/Game.h>
#include <cmath>
#include <utility>
#include <vector>


namespace carcassonne::ai {

Node::Node(rl::PoolGame &&game, const Player &player, FullMove move)
    : m_game(std::move(game)), m_mutex(std::make_unique<std::mutex>()), m_player(player), m_move(move) {}

Node::Node(rl::PoolGame &&game, const Player &player, FullMove move, float P, NodePtr parent_ptr)
    : m_game(std::move(game)), m_mutex(std::make_unique<std::mutex>()), m_player(player), m_parent(parent_ptr), m_move(move), m_P(P) {}

double Node::UCT1(mb::size rollout_count) const noexcept {
   const auto player_wins = m_player_wins[static_cast<int>(m_player)];
   if (m_simulation_count == 0) {
      return 1.0;
   }
   return static_cast<double>(player_wins) / static_cast<double>(m_simulation_count) + g_C * std::sqrt(std::log(static_cast<double>(rollout_count)) / static_cast<double>(m_simulation_count));
}

double Node::PUCT() const noexcept {
   if (m_simulation_count == 0) {
      return 1.0;
   }
   return g_C * m_P * std::sqrt(static_cast<double>(m_parent->simulation_count()) / (1 + static_cast<double>(m_simulation_count)));
}

NodePtr Node::add_child(rl::PoolGame &&game, const Player &player, FullMove move, float P) noexcept {
   std::lock_guard<std::mutex> lock(*m_mutex);
   m_children.push_back(std::make_unique<Node>(std::move(game), player, move, P, this));
   return m_children.back().get();
}

Player Node::find_winner() {
   auto max_score_it = std::max_element(m_game->scores().begin(), m_game->scores().end(), [](PlayerScore lhs, PlayerScore rhs) {
      return lhs.score < rhs.score;
   });
   return max_score_it->player;
}

IGame &Node::game() const noexcept {
   return *m_game;
}

[[nodiscard]] std::size_t Node::count_children() const {
   return std::accumulate(m_children.begin(), m_children.end(), static_cast<std::size_t>(1), [](std::size_t acc, const std::unique_ptr<Node> &node) {
      return acc + node->count_children();
   });
}

}// namespace carcassonne::ai