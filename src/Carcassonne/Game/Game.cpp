#include <Carcassonne/Game/Game.h>
#include <Carcassonne/Game/Move.h>
#include <algorithm>

namespace carcassonne::game {

Game::Game() : m_random_generator(6) {
   apply_tile(70, 70, 1, 3);
}

const IBoard &Game::board() const noexcept {
   return m_board;
}

Player Game::current_player() const noexcept {
   return m_current_player;
}

std::unique_ptr<IMove> Game::new_move(Player p) noexcept {
   return std::make_unique<Move>(p, (m_random_distribution(m_random_generator) % 24) + 1, *this);
}

mb::view<Figure> Game::figures() const noexcept {
   return m_figures;
}

void Game::add_figure(Figure f) {
   m_figures.push_back(f);
}

std::unique_ptr<IMove> Game::new_debug_move(Player p, TileType tt) noexcept {
   return std::make_unique<Move>(p, tt, *this);
}

void Game::apply_tile(int x, int y, TileType tt, mb::u8 rot) noexcept {
   m_board.set_tile(x, y, tt, rot);

   auto tile = TilePlacement{.type = tt, .rotation = rot}.tile();
   auto connections = tile.connections;
   while (connections != Connection::None) {
      Direction a, b;
      std::tie(connections, a, b) = read_directions(connections);
      m_groups.join(make_edge(x, y, a), make_edge(x, y, b));
   }

   for (int direction_id = 0; direction_id < 4; ++direction_id) {
      m_groups.set_type(make_edge(x, y, static_cast<Direction>(direction_id)), tile.edges[direction_id]);
   }
   for (int direction_id = 5; direction_id < 13; ++direction_id) {
      m_groups.set_type(make_edge(x, y, static_cast<Direction>(direction_id)), tile.field_edges[direction_id-5]);
   }
}

}// namespace carcassonne::game