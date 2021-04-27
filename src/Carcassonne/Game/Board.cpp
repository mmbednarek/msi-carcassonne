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
   if (x < 0 || y < 0 || x >= g_board_width || y >= g_board_height)
      return TilePlacement{.type = 0};
   return m_board[x + y * g_board_width];
}

void Board::set_tile(int x, int y, TileType t, mb::u8 rotation) noexcept {
   if (x < 0 || y < 0 || x >= g_board_width || y >= g_board_height)
      return;

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

bool Board::can_place_at(int x, int y, TileType t, mb::u8 rotation) const noexcept {
   if (x < 0 || y < 0 || x >= g_board_width || y >= g_board_height)
      return false;

   if (tile_at(x, y).type != 0)
      return false;

   int zeros = 0;

   auto main_tile = g_tiles[t].rotate(rotation);
   if (auto placement = tile_at(x, y-1); placement.type != 0) {
      auto tile = placement.tile();
      if (main_tile.edges[0] != tile.edges[2])
         return false;
   } else ++zeros;
   if (auto placement = tile_at(x, y+1); placement.type != 0) {
      auto tile = placement.tile();
      if (main_tile.edges[2] != tile.edges[0])
         return false;
   } else ++zeros;
   if (auto placement = tile_at(x-1, y); placement.type != 0) {
      auto tile = placement.tile();
      if (main_tile.edges[3] != tile.edges[1])
         return false;
   } else ++zeros;
   if (auto placement = tile_at(x+1, y); placement.type != 0) {
      auto tile = placement.tile();
      if (main_tile.edges[1] != tile.edges[3])
         return false;
   } else ++zeros;

   if (zeros == 4)
      return false;

   return true;
}

}// namespace carcassonne::game