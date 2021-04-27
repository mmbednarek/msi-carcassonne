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

   auto tile_x = (m_current_tile - 1) % 4;
   auto tile_y = (m_current_tile - 1) / 4;

   ctx.draw(ResourceManager::texture(TextureResource::Tiles),
            g_tile_size * tile_x,
            g_tile_size * tile_y,
            g_tile_size,
            g_tile_size,
            static_cast<int>(m_scale * (m_selected_tile_x - m_cam_x)),
            static_cast<int>(m_scale * (m_selected_tile_y - m_cam_y)),
            static_cast<int>(m_scale),
            static_cast<int>(m_scale),
            90 * m_rotation);

   if (m_selection_ok) {
      ctx.set_draw_color(0, 255, 0, 60 + static_cast<int>(m_light));
   } else {
      ctx.set_draw_color(255, 0, 0, 60 + static_cast<int>(m_light));
   };

   ctx.draw_box(static_cast<int>(m_scale * (m_selected_tile_x - m_cam_x)),
                static_cast<int>(m_scale * (m_selected_tile_y - m_cam_y)),
                static_cast<int>(m_scale),
                static_cast<int>(m_scale));
}

void BoardView::update(double dt) {
   if (m_ligth_down) {
      m_light -= 0.1 * dt;
      if (m_light < 0)
         m_ligth_down = false;
   } else {
      m_light += 0.1 * dt;
      if (m_light > 40)
         m_ligth_down = true;
   }
}

void BoardView::scale(double amount) {
   if (amount > 0.0 && m_scale > 256.0)
      return;
   if (amount < 0.0 && m_scale < 36.0)
      return;
   m_scale += amount;
}

void BoardView::move_cam(double x, double y) {
   if (x < 0.0 && m_cam_x < (m_board.min_x() - 8))
      return;
   if (y < 0.0 && m_cam_y < (m_board.min_y() - 8))
      return;
   if (x > 0.0 && m_cam_x > m_board.max_x())
      return;
   if (y > 0.0 && m_cam_y > m_board.max_y())
      return;
   m_cam_x += x / m_scale;
   m_cam_y += y / m_scale;
}

bool BoardView::place_tile() {
   if (!m_selection_ok)
      return false;

   m_board.set_tile(m_selected_tile_x, m_selected_tile_y, m_current_tile, m_rotation);
   return true;
}

void BoardView::rotate_tile() {
   m_rotation = (m_rotation + 1) % 4;
   m_selection_ok = m_board.can_place_at(m_selected_tile_x, m_selected_tile_y, m_current_tile, m_rotation);
}

void BoardView::select_tile(double x, double y) {
   const auto board_x = static_cast<int>(x / m_scale + m_cam_x);
   const auto board_y = static_cast<int>(y / m_scale + m_cam_y);
   if (board_x != m_selected_tile_x || board_y != m_selected_tile_y) {
      m_selected_tile_x = board_x;
      m_selected_tile_y = board_y;
      m_selection_ok = m_board.can_place_at(board_x, board_y, m_current_tile, m_rotation);
   }
}

void BoardView::set_selected_tile_type(TileType t) {
   m_current_tile = t;
   m_rotation = 0;
}

}// namespace carcassonne::frontend