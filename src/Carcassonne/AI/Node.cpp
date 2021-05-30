#include <Carcassonne/AI/Node.h>


namespace carcassonne::ai {

Node::Node(IGame &game, const Player &player, mb::u64 &rollouts_performed_count, std::shared_ptr<Node> parent)
 : m_game(game)
 , m_player(player)
 , m_parent(parent) {}

void Node::expansion(mb::u64 &rollouts_performed_count) noexcept {
   auto move = m_game.new_move(m_player);
   const auto possible_tile_moves = m_game.moves(move->tile_type());
   for (TileMove possible_tile_move : possible_tile_moves) {
      auto game_clone = m_game.clone();
      auto move = game_clone->new_move(m_player);
      move->place_tile(possible_tile_move.x, possible_tile_move.y, possible_tile_move.rotation);
      const auto possible_figure_moves = game_clone->figure_placements(possible_tile_move.x, possible_tile_move.y);
      for (Direction possible_figure_move : possible_figure_moves) {
         auto game_clone_clone = game_clone->clone();
         auto move_clone = game_clone_clone->new_move(m_player);
         move_clone->place_figure(possible_figure_move);
         auto parent = std::make_shared<Node>(std::move(*this));
         Node n(*game_clone_clone, m_player, rollouts_performed_count, parent);
         std::shared_ptr<Node> nptr = std::make_shared<Node>(n);
         m_children.push_back(nptr);
      }
   }
}

std::tuple<std::size_t, std::size_t> Node::simulation() noexcept {
   // IN PROGRESS
   // RandomPlayer rp = RandomPlayer(game, m_player);
   std::size_t wins_count = 0;
   std::size_t losess_count = 0;

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

   
   return std::make_tuple(wins_count, losess_count);
}

double Node::UCT1() noexcept {
   // DONE
   if (m_visitation_count == 0) {
      return std::numeric_limits<double>::infinity();
   }
   return m_wins_count / m_loses_count + g_C * sqrt(log(m_max_id) / m_visitation_count);
}

}// namespace carcassonne::ai