#include <Carcassonne/AI/Node.h>

#include <utility>


namespace carcassonne::ai {

Node::Node(std::unique_ptr<IGame> game, const Player &player, mb::u64 &rollouts_performed_count)
 : m_game(std::move(game))
 , m_player(player) {}

Node::Node(std::unique_ptr<IGame> game, const Player &player, mb::u64 &rollouts_performed_count, Node *parent)
 : m_game(std::move(game))
 , m_player(player)
 , m_parent(parent) {}

void Node::simulation() noexcept {
   // IN PROGRESS
   // RandomPlayer rp = RandomPlayer(game, m_player);
   std::size_t wins_count = 0;
   std::size_t losess_count = 0;
   ++m_visitation_count;

   // SIMULATION BEGINS HERE
   // auto it_player = std::find_if(m_game.scores().begin(), m_game.scores().end(),
   //                        [this](PlayerScore score) { return score.player == this->m_player; });
   // auto player = (*g_players.begin());
   // auto it_oponent = std::find_if(m_game.scores().begin(), m_game.scores().end(),
   //                        [this](PlayerScore score) { return score.player != this->m_player; });
   // if (it_player == m_game.scores().end()) {
   //    return std::make_pair(0, 0);
   // }
   // if (it_player->score > it_oponent->score)
   //    wins_count++;
   // else
   //    losess_count++;

   
   m_wins_count = wins_count + 1;
   m_loses_count = losess_count + 1;
}

double Node::UCT1(mb::u64 &rollouts_performed_count) noexcept {
   // DONE
   if (m_visitation_count == 0 || m_loses_count == 0 || rollouts_performed_count == 0) {
      return std::numeric_limits<double>::infinity();
   }
   return m_wins_count / m_loses_count + g_C * sqrt(log(rollouts_performed_count) / m_visitation_count);
}

}// namespace carcassonne::ai