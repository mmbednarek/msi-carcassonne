#include <Carcassonne/AI/Node.h>
#include <Carcassonne/AI/RandomPlayer.h>
#include <utility>
#include <vector>


namespace carcassonne::ai {

Node::Node(std::unique_ptr<IGame> game, const Player &player, FullMove move)
    : m_game(std::move(game)), m_player(player), m_move(move), m_id(0) {}

Node::Node(NodeId id, std::unique_ptr<IGame> game, const Player &player, FullMove move, NodeId parent_id)
    : m_game(std::move(game)), m_player(player), m_move(move), m_parent_id(parent_id), m_id(id) {}


std::random_device g_random_device;
std::mt19937 g_random_generator(g_random_device());

void Node::simulation() noexcept {
   // IN PROGRESS
   ++m_visitation_count;

   const auto player_count = m_game->player_count();


   std::vector<RandomPlayer<>> players;
   players.reserve(player_count);
   std::transform(g_players.begin(), g_players.begin() + player_count, std::back_inserter(players), [](carcassonne::Player p) {
      return carcassonne::ai::RandomPlayer<>(g_random_generator, p);
   });

   auto current_player = m_game->current_player();

   auto simulated_game = m_game->clone();
   for (auto move_index = simulated_game->move_index(); move_index < g_max_moves; ++move_index) {
      players[static_cast<mb::size>(current_player)].make_move(*simulated_game);
      current_player = next_player(current_player, player_count);
      simulated_game->update(0);
   }

   auto player_it = std::find_if(simulated_game->scores().begin(), simulated_game->scores().end(),
                                 [player = m_player](PlayerScore score) { return score.player == player; });
   auto max_score_it = std::max_element(simulated_game->scores().begin(), simulated_game->scores().end(), [](PlayerScore lhs, PlayerScore rhs) {
      return lhs.score < rhs.score;
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