#include <Carcassonne/Frontend/BoardView.h>
#include <Carcassonne/Frontend/Resource.h>
#include <Carcassonne/Frontend/Tile.h>

namespace carcassonne::frontend {

BoardView::BoardView(IBoard &board, Camera &camera) : m_board(board), m_camera(camera) {}

void BoardView::render(const graphics::Context &ctx) const noexcept {
   for (int y = m_board.min_y(); y < m_board.max_y(); ++y) {
      for (int x = m_board.min_x(); x < m_board.max_x(); ++x) {
         auto tile = m_board.tile_at(x, y);
         if (tile.type == 0)
            continue;

         render_tile(ctx, m_camera, x, y, tile.type, tile.rotation);
      }
   }
}

void BoardView::update(double dt) {
}

}// namespace carcassonne::frontend