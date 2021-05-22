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

   if (m_game.player_figure_count(m_player) == 0 || std::all_of(g_directions.cbegin(), g_directions.cend(), [this](Direction d) { return !is_free(d); })) {
      m_phase = MovePhase::Done;
      m_game.set_next_player();
   }
}

void Move::place_figure(Direction d) noexcept {
   if (m_phase != MovePhase::PlaceFigure)
      return;

   if (!is_free(d))
      return;

   auto edge = make_edge(m_x, m_y, d);

   double px, py;
   std::tie(px, py) = direction_position(TilePosition{m_x, m_y}, d);
   m_game.add_figure(Figure{
           .player = m_player,
           .x = px,
           .y = py,
           .tile_x = m_x,
           .tile_y = m_y,
           .edge = edge,
   });

   m_game.mutable_groups().assign(make_edge(m_x, m_y, d), m_player);

   if (is_side_direction(d) && m_game.groups().is_completed(edge)) {
      m_game.on_structure_completed(m_game.groups().group_of(edge));
   } else if (d == Direction::Middle && m_game.is_monastery_completed(m_x, m_y)) {
      m_game.on_monastery_completed(m_x, m_y, m_player);
   }

   m_phase = MovePhase::Done;
   m_game.set_next_player();
}

void Move::ignore_figure() noexcept {
   if (m_phase != MovePhase::PlaceFigure)
      return;
   m_phase = MovePhase::Done;
   m_game.set_next_player();
}

MovePhase Move::phase() const noexcept {
   return m_phase;
}

TilePosition Move::position() const noexcept {
   return TilePosition{m_x, m_y};
}

bool Move::is_free(Direction d) const noexcept {
   return m_game.can_place_figure(m_x, m_y, d);
}

}// namespace carcassonne::game
