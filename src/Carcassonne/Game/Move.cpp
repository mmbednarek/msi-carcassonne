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

   m_game.apply_tile(x, y, m_tile_type, rotation);

   bool all_occupied = true;
   for (const auto d : g_directions) {
      if (is_free(d)) {
         all_occupied = false;
         break;
      }
   }

   if (all_occupied) {
      return;
   }
}

void Move::place_figure(Direction d) noexcept {
   if (m_phase != MovePhase::PlaceFigure)
      return;

   if (!is_free(d))
      return;

   auto edge = make_edge(m_x, m_y, d);

   double px{}, py{};
   std::tie(px, py) = direction_position(TilePosition{m_x, m_y}, d);
   m_game.add_figure(Figure{
           .player = m_player,
           .x = px,
           .y = py,
           .edge = edge,
   });

   m_game.mutable_groups().assign(make_edge(m_x, m_y, d), m_player);
   if (m_game.groups().is_completed(edge) && is_side_direction(d)) {
      m_game.on_structure_completed(m_x, m_y, m_game.groups().group_of(edge));
   }

   if (d == Direction::Middle) {
      if(m_game.is_monastery_completed(m_x, m_y)) {
         m_game.on_monastery_completed(m_x, m_y, m_player);
      }
   }

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

   auto edge = make_edge(m_x, m_y, d);
   auto edge_type = m_game.groups().type_of(edge);

   switch (d) {
   case Direction::North:
   case Direction::East:
   case Direction::South:
   case Direction::West:
      if (edge_type == EdgeType::Grass)
         return false;
      break;
   case Direction::EastNorth:
   case Direction::NorthEast:
   case Direction::SouthEast:
   case Direction::EastSouth:
   case Direction::WestSouth:
   case Direction::SouthWest:
   case Direction::NorthWest:
   case Direction::WestNorth:
      if (edge_type != EdgeType::Grass)
         return false;
      break;
   }

   return m_game.groups().is_free(edge);
}

}// namespace carcassonne::game
