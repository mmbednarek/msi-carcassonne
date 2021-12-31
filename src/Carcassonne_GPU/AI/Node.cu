#ifdef __CUDACC__
#define DEVHOST __device__ __host__
#else
#define DEVHOST
#endif

#include <Carcassonne_GPU/AI/Node.h>
#include <Carcassonne_GPU/ScoreBoard.h>
#include <cmath>
#include <utility>
#include <vector>

namespace carcassonne::ai {

DEVHOST Node::Node(game::Game &&game, const Player &player, game::FullMove move)
    : m_game(std::move(game)), m_player(player), m_move(move), m_id(0) {}

DEVHOST Node::Node(NodeId id, game::Game &&game, const Player &player, game::FullMove move, NodeId parent_id)
    : m_game(std::move(game)), m_player(player), m_move(move), m_parent_id(parent_id), m_id(id) {}

DEVHOST double Node::UCT1(mb::size rollout_count) const noexcept {
   const auto player_wins = m_player_wins[static_cast<int>(m_player)];
   if (m_simulation_count == 0) {
      return 1.0;
   }
   return static_cast<double>(player_wins) / static_cast<double>(m_simulation_count) + g_C * std::sqrt(std::log(static_cast<double>(rollout_count)) / static_cast<double>(m_simulation_count));
}

DEVHOST void Node::add_child(NodeId id) noexcept {
   m_children.push_back(id);
}

DEVHOST Player Node::find_winner() {
   auto max_score_it = std::max_element(m_game.scores().begin(), m_game.scores().end(), [](PlayerScore lhs, PlayerScore rhs) {
      return lhs.score < rhs.score;
   });
   return max_score_it->player;
}

DEVHOST game::Game &Node::game() const noexcept {
   return m_game;
}

}// namespace carcassonne::ai