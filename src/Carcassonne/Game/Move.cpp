#include <Carcassonne/Game/Game.h>
#include <Carcassonne/Game/Move.h>

namespace carcassonne::game {

Player Move::player() const noexcept {
   return m_player;
}

TileType Move::tile_type() const noexcept {
   return m_tile_type;
}

void Move::place_tile(int x, int y, mb::u8 rotation) noexcept {
   if (m_phase != MovePhase::PlaceTile)
      return;

   m_x = x;
   m_y = y;
   m_phase = MovePhase::PlaceFigure;

   m_game.mutable_board().set_tile(x, y, m_tile_type, rotation);

   auto connections = TilePlacement{.type = m_tile_type, .rotation = rotation}.tile().connections;
   while (connections != Connection::None) {
      Direction a, b;
      std::tie(connections, a, b) = read_directions(connections);
      m_game.mutable_groups().join(make_edge(x, y, a), make_edge(x, y, b));
   }

   bool all_occupied = true;
   for (const auto d : g_directions) {
      if (is_free(d)) {
         all_occupied = false;
         break;
      }
   }

   if (all_occupied) {
      m_game.set_next_player();
      m_phase = MovePhase::Done;
   }
}

void Move::place_figure(Direction d) noexcept {
   if (m_phase != MovePhase::PlaceFigure)
      return;

   if (!is_free(d))
      return;

   double px{}, py{};
   std::tie(px, py) = direction_position(TilePosition{m_x, m_y}, d);
   m_game.add_figure(Figure{
           .player = m_player,
           .x = px,
           .y = py,
   });

   m_game.mutable_groups().assign(make_edge(m_x, m_y, d), static_cast<mb::u8>(m_player));
   m_game.set_next_player();
   m_phase = MovePhase::Done;
}

void Move::ignore_figure() noexcept {
   if (m_phase != MovePhase::PlaceFigure)
      return;
   m_game.set_next_player();
   m_phase = MovePhase::Done;
}

MovePhase Move::phase() const noexcept {
   return m_phase;
}

TilePosition Move::position() const noexcept {
   return TilePosition{m_x, m_y};
}

bool Move::is_free(Direction d) const noexcept {
   if (d == Direction::Middle) {
      return m_game.board().tile_at(m_x, m_y).tile().monastery;
   }
   return m_game.groups().is_free(make_edge(m_x, m_y, d));
}

}// namespace carcassonne::game
