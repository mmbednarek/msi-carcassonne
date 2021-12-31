#ifdef __CUDACC__
#define DEVHOST __device__ __host__
#else
#define DEVHOST
#endif
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#include <Carcassonne_GPU/Game/Game.h>
#include <Carcassonne_GPU/Game/Move.h>
#include <memory>

namespace carcassonne::game {

DEVHOST Player Move::player() const noexcept {
   return m_player;
}

DEVHOST TileType Move::tile_type() const noexcept {
   return m_tile_type;
}

DEVHOST bool Move::place_tile_at(int x, int y, mb::u8 rotation) noexcept {
   if (m_phase != MovePhase::PlaceTile)
      return EXIT_FAILURE;

   if (m_game.board().can_place_at(x, y, m_tile_type, rotation)) {
      // fmt::print("OU OU... x={}, y={}, rot={}\n", x, y, rotation);
      return EXIT_FAILURE;
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
   return EXIT_SUCCESS;
}

DEVHOST bool Move::place_figure(Direction d) noexcept {
   if (m_phase != MovePhase::PlaceFigure) {
      return EXIT_FAILURE;
   }

   if (!is_free(d)) {
      return EXIT_FAILURE;
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
   return EXIT_SUCCESS;
}

DEVHOST bool Move::ignore_figure() noexcept {
   if (m_phase != MovePhase::PlaceFigure)
      return EXIT_FAILURE;
   m_phase = MovePhase::Done;
   m_game.notify_tour_finished(FullMove{
           .x = m_x,
           .y = m_y,
           .rotation = m_rotation,
           .ignored_figure = true,
   });
   return EXIT_SUCCESS;
}

DEVHOST MovePhase Move::phase() const noexcept {
   return m_phase;
}

DEVHOST TilePosition Move::position() const noexcept {
   return TilePosition{m_x, m_y};
}

DEVHOST bool Move::is_free(Direction d) const noexcept {
   return m_game.can_place_figure(m_x, m_y, d);
}

DEVHOST ecuda::unique_ptr<Move> Move::clone(Game &game) const noexcept {
   return ecuda::unique_ptr<Move>(new Move{game, m_player, m_tile_type, m_x, m_y});
}

DEVHOST bool Move::place_tile(TileMove tile_location) noexcept {
   return place_tile_at(tile_location.x, tile_location.y, tile_location.rotation);
}

}
// namespace carcassonne::game
