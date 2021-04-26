#include <Carcassonne/Game/Board.h>

namespace carcassonne::game {

Board::Board() {
   m_board[70 + 70 * g_board_width] = TilePlacement{
           .type = 1,
           .rotation = 3,
   };
}

int Board::min_x() const noexcept {
   return m_min_x;
}

int Board::min_y() const noexcept {
   return m_min_y;
}

int Board::max_x() const noexcept {
   return m_max_x;
}

int Board::max_y() const noexcept {
   return m_max_y;
}

TilePlacement Board::tile_at(int x, int y) const noexcept {
   return m_board[x + y * g_board_width];
}

void Board::set_tile(int x, int y, TileType t, mb::u8 rotation) noexcept {
   if (x < m_min_x) {
      m_min_x = x;
   } else if (x >= m_max_x) {
      m_max_x = x+1;
   }
   if (y < m_min_y) {
      m_min_y = y;
   } else if (y >= m_max_y) {
      m_max_y = y+1;
   }

   m_board[x + y * g_board_width] = TilePlacement{
           .type = t,
           .rotation = rotation,
   };
}

}// namespace carcassonne::game