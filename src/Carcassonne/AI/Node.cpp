#include <Carcassonne/AI/Node.h>


namespace carcassonne::ai {

Node::Node(IGame &game, const Player &player, mb::u64 &simulations_count)
 : m_game(game)
 , m_player(player) {}

std::vector<Node> Node::expansion(mb::u64 &simulations_count) {
   auto move = m_game.new_move(m_player);
   const auto possible_tile_moves_range = m_game.moves(move->tile_type());
   const std::vector<TileMove> possible_tile_moves(possible_tile_moves_range.begin(), possible_tile_moves_range.end() - 1);
   std::vector<Node> children;
   for (TileMove possible_tile_move : possible_tile_moves) {
      auto game_clone = m_game.clone();
      auto move = (*game_clone).new_move(m_player);
      move->place_tile(possible_tile_move.x, possible_tile_move.y, possible_tile_move.rotation);
      const auto possible_figure_moves_range = (*game_clone).figure_placements(possible_tile_move.x, possible_tile_move.y);
      const std::vector<Direction> possible_figure_moves(possible_figure_moves_range.begin(), possible_figure_moves_range.end() - 1);
      for (Direction possible_figure_move : possible_figure_moves) {
         auto game_clone_clone = (*game_clone).clone();
         auto move_clone = (*game_clone_clone).new_move(m_player);
         move_clone->place_figure(possible_figure_move);
         children.push_back(Node(*game_clone_clone, m_player, simulations_count));
      }
   }
   return children;
}

short Node::simulation(IGame &game) {
   // IN PROGRESS
   // RandomPlayer rp = RandomPlayer(game, m_player);
   auto it = std::find_if(game.scores().begin(), game.scores().end(),
                          [this](PlayerScore score) { return score.player == this->m_player; });
   if (it == game.scores().end()) {
      return 0;
   }
   return it->score;
}

double Node::UCT1(Node node) {
   // DONE
   if (m_visitation_count == 0) {
      return std::numeric_limits<double>::infinity();
   }
   return m_wins_count / m_loses_count + g_C * sqrt(log(m_max_id) / m_visitation_count);
}

}// namespace carcassonne::ai