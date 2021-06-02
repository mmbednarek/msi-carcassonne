#include <Carcassonne/AI/Node.h>
#include <Carcassonne/AI/RandomPlayer.h>
#include <utility>
#include <vector>


namespace carcassonne::ai {

Node::Node(std::unique_ptr<IGame> game, const Player &player, FullMove move)
    : m_game(std::move(game)), m_player(player), m_move(move), m_id(0) {}

Node::Node(NodeId id, std::unique_ptr<IGame> game, const Player &player, FullMove move, NodeId parent_id)
    : m_game(std::move(game)), m_player(player), m_move(move), m_parent_id(parent_id), m_id(id) {}

void Node::simulation() noexcept {
   // IN PROGRESS
   ++m_visitation_count;

   const auto player_count = m_game->player_count();

   std::random_device random_device;
   std::mt19937 random_generator(random_device());

   std::vector<RandomPlayer<>> players;
   players.reserve(player_count);
   std::transform(g_players.begin(), g_players.begin() + player_count, std::back_inserter(players), [&random_generator](carcassonne::Player p) {
     return carcassonne::ai::RandomPlayer<>(random_generator, p);
   });

   auto current_player = m_game->current_player();
   for (auto move_index = m_game->move_index(); move_index < g_max_moves; ++move_index) {
      players[static_cast<mb::size>(current_player)].make_move(*m_game);
      current_player = next_player(current_player, player_count);
      m_game->update(0);
   }

   auto player_it = std::find_if(m_game->scores().begin(), m_game->scores().end(),
                                 [player = m_player](PlayerScore score) { return score.player == player; });
   auto max_score_it = std::max_element(m_game->scores().begin(), m_game->scores().end(), [](PlayerScore lhs, PlayerScore rhs) {
      return lhs.score > rhs.score;
   });

   if (player_it->score == max_score_it->score) {
      m_wins_count += 1;
   } else {
      m_loses_count += 1;
   }
}

double Node::UCT1(mb::u64 &rollouts_performed_count) const noexcept {
   // DONE
   if (m_visitation_count == 0 || m_loses_count == 0 || rollouts_performed_count == 0) {
      return std::numeric_limits<double>::infinity();
   }
   return static_cast<double>(m_wins_count) / static_cast<double>(m_loses_count) + g_C * std::sqrt(std::log(rollouts_performed_count) / static_cast<double>(m_visitation_count));
}

void Node::add_child(NodeId id) noexcept {
   m_children.push_back(id);
}

}// namespace carcassonne::ai