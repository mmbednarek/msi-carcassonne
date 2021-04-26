#include <Carcassonne/Frontend/BoardView.h>
#include <Carcassonne/Frontend/Resource.h>

namespace carcassonne::frontend {

constexpr auto g_tile_size = 96;

BoardView::BoardView(IBoard &board) : m_board(board) {}

void BoardView::render(const graphics::Context &ctx) const noexcept {
   for (int y = m_board.min_y(); y < m_board.max_y(); ++y) {
      for (int x = m_board.min_x(); x < m_board.max_x(); ++x) {
         auto tile = m_board.tile_at(x, y);
         if (tile.type == 0)
            continue;

         auto tile_x = (tile.type - 1) % 4;
         auto tile_y = (tile.type - 1) / 4;

         ctx.draw(ResourceManager::texture(TextureResource::Tiles),
                  g_tile_size * tile_x,
                  g_tile_size * tile_y,
                  g_tile_size,
                  g_tile_size,
                  static_cast<int>(m_scale * (x - m_cam_x)),
                  static_cast<int>(m_scale * (y - m_cam_y)),
                  static_cast<int>(m_scale),
                  static_cast<int>(m_scale),
                  90 * tile.rotation);
      }
   }
}

void BoardView::scale(double amount) {
   m_scale += amount;
}

void BoardView::move_cam(double x, double y) {
   m_cam_x += x / m_scale;
   m_cam_y += y / m_scale;
}

void BoardView::place_random_tile(double x, double y) {
   const auto board_x = static_cast<int>(x / m_scale + m_cam_x);
   const auto board_y = static_cast<int>(y / m_scale + m_cam_y);
   m_board.set_tile(board_x, board_y, (rand() % 24) + 1, rand() % 4);
}

void BoardView::rotate_tile(double x, double y) {
   const auto board_x = static_cast<int>(x / m_scale + m_cam_x);
   const auto board_y = static_cast<int>(y / m_scale + m_cam_y);
   auto tile = m_board.tile_at(board_x, board_y);
   m_board.set_tile(board_x, board_y, tile.type, (tile.rotation + 1) % 4);
}

}// namespace carcassonne::frontend