#include <Carcassonne/AI/HeuristicPlayer.h>
#include <Carcassonne/AI/Node.h>
#include <utility>
#include <vector>


namespace carcassonne::ai {

Node::Node(std::unique_ptr<IGame> game, const Player &player, FullMove move)
    : m_game(std::move(game)), m_player(player), m_move(move), m_id(0) {}

Node::Node(NodeId id, std::unique_ptr<IGame> game, const Player &player, FullMove move, NodeId parent_id)
    : m_game(std::move(game)), m_player(player), m_move(move), m_parent_id(parent_id), m_id(id) {}

void Node::simulation() noexcept {
   ++m_visitation_count;

   const auto player_count = m_game->player_count();

   std::vector<HeuristicPlayer> players;
   players.reserve(player_count);
   std::transform(g_players.begin(), g_players.begin() + player_count, std::back_inserter(players), [](carcassonne::Player p) {
      return HeuristicPlayer(p);
   });

   auto simulated_game = m_game->clone();
   for (auto move_index = simulated_game->move_index(); move_index < g_max_moves; ++move_index) {
      players[static_cast<mb::size>(simulated_game->current_player())].make_move(*simulated_game);
      simulated_game->update(0);
   }

   auto max_score_it = std::max_element(simulated_game->scores().begin(), simulated_game->scores().end(), [](PlayerScore lhs, PlayerScore rhs) {
      return lhs.score < rhs.score;
   });

   m_player_wins[static_cast<int>(max_score_it->player)] += 1;
}

double Node::UCT1(mb::size rollout_count) const noexcept {
   const auto player_wins = m_player_wins[static_cast<int>(m_player)];
   if (m_visitation_count == player_wins || rollout_count == 0) {
      return std::numeric_limits<double>::infinity();
   }
   return static_cast<double>(player_wins) / static_cast<double>(m_visitation_count - player_wins) + g_C * std::sqrt(std::log(static_cast<double>(rollout_count)) / static_cast<double>(m_visitation_count));
}

void Node::add_child(NodeId id) noexcept {
   m_children.push_back(id);
}

}// namespace carcassonne::ai