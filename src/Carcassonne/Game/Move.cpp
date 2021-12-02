#include <Carcassonne/Game/Game.h>
#include <Carcassonne/Game/Move.h>
#include <Carcassonne/Move.h>
#include <memory>

namespace carcassonne::game {

Player Move::player() const noexcept {
   return m_player;
}

TileType Move::tile_type() const noexcept {
   return m_tile_type;
}

mb::result<mb::empty> Move::place_tile_at(int x, int y, mb::u8 rotation) noexcept {
   if (m_phase != MovePhase::PlaceTile)
      return mb::error("invalid move phase");

   if (!m_game.board().can_place_at(x, y, m_tile_type, rotation)) {
      fmt::print("OU OU... x={}, y={}, rot={}\n", x, y, rotation);
      return mb::error("cannot place tile at this location");
   }

   m_x = x;
   m_y = y;
   m_rotation = rotation;
   m_phase = MovePhase::PlaceFigure;

   m_game.apply_tile(x, y, m_tile_type, rotation);

   if (m_game.player_figure_count(m_player) == 0 || std::all_of(g_directions.cbegin(), g_directions.cend(), [this](Direction d) { return !is_free(d); })) {
      m_phase = MovePhase::Done;
      m_game.notify_tour_finished(FullMove{
              .x = m_x,
              .y = m_y,
              .rotation = m_rotation,
              .ignored_figure = true,
      });
   }
   return mb::ok;
}

mb::result<mb::empty> Move::place_figure(Direction d) noexcept {
   if (m_phase != MovePhase::PlaceFigure) {
      return mb::error("incorrect move phase, place the tile first");
   }

   if (!is_free(d)) {
      return mb::error("given direction is occupied");
   }

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
           .dir = d,
   });

   m_game.mutable_groups().assign(make_edge(m_x, m_y, d), m_player);

   if (is_side_direction(d) && m_game.groups().is_completed(edge)) {
      m_game.on_structure_completed(m_game.groups().group_of(edge));
   } else if (d == Direction::Middle && m_game.is_monastery_completed(m_x, m_y)) {
      m_game.on_monastery_completed(m_x, m_y, m_player);
   }

   m_phase = MovePhase::Done;
   m_game.notify_tour_finished(FullMove{
           .x = m_x,
           .y = m_y,
           .rotation = m_rotation,
           .ignored_figure = false,
           .direction = d,
   });
   return mb::ok;
}

mb::result<mb::empty> Move::ignore_figure() noexcept {
   if (m_phase != MovePhase::PlaceFigure)
      return mb::error("incorrect move phase, place the tile first");
   m_phase = MovePhase::Done;
   m_game.notify_tour_finished(FullMove{
           .x = m_x,
           .y = m_y,
           .rotation = m_rotation,
           .ignored_figure = true,
   });
   return mb::ok;
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

std::unique_ptr<IMove> Move::clone(IGame &game) const noexcept {
   return std::make_unique<Move>(*dynamic_cast<Game *>(&game), m_player, m_tile_type, m_x, m_y);
}

mb::result<mb::empty> Move::place_tile(TileMove tile_location) noexcept {
   return place_tile_at(tile_location.x, tile_location.y, tile_location.rotation);
}

}
// namespace carcassonne::game
