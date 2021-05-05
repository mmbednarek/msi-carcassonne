#include <Carcassonne/Frontend/Object.h>
#include <Carcassonne/Frontend/PlaceTileView.h>

namespace carcassonne::frontend {

PlaceTileView::PlaceTileView(const IBoard &board, const Camera &camera, std::unique_ptr<IMove> &move) : m_board(board),
                                                                                                        m_camera(camera),
                                                                                                        m_move(move) {}

void PlaceTileView::render(const graphics::Context &ctx) const {
   auto tile_type = m_move->tile_type();

   render_tile(ctx, m_camera, m_tile_x, m_tile_y, tile_type, m_rotation);

   if (m_selection_ok) {
      ctx.set_draw_color(67, 182, 76, 90 + static_cast<int>(m_flicker));
   } else {
      ctx.set_draw_color(182, 67, 83, 120 + static_cast<int>(m_flicker));
   }

   ctx.draw_box(static_cast<int>(m_camera.translate_x(m_tile_x)),
                static_cast<int>(m_camera.translate_y(m_tile_y)),
                static_cast<int>(m_camera.scale(1.0)),
                static_cast<int>(m_camera.scale(1.0)));
}

void PlaceTileView::update(double dt) {
   if (m_flicker_raising) {
      m_flicker += 0.1 * dt;
      if (m_flicker > 40.0)
         m_flicker_raising = false;
   } else {
      m_flicker -= 0.1 * dt;
      if (m_flicker < 0.0)
         m_flicker_raising = true;
   }
}

bool PlaceTileView::place_tile() {
   if (!m_selection_ok)
      return false;

   m_move->place_tile(m_tile_x, m_tile_y, m_rotation);
   m_selection_ok = false;
   return true;
}

void PlaceTileView::rotate_tile() {
   m_rotation = (m_rotation + 1) % 4;
   m_selection_ok = m_board.can_place_at(m_tile_x, m_tile_y, m_move->tile_type(), m_rotation);
}

void PlaceTileView::select_tile(double x, double y) {
   const auto board_x = static_cast<int>(m_camera.reverse_x(x));
   const auto board_y = static_cast<int>(m_camera.reverse_y(y));
   if (board_x != m_tile_x || board_y != m_tile_y) {
      m_tile_x = board_x;
      m_tile_y = board_y;
      m_selection_ok = m_board.can_place_at(board_x, board_y, m_move->tile_type(), m_rotation);
   }
}

}// namespace carcassonne::frontend