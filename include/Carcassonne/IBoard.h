#ifndef MSI_CARCASSONNE_IBOARD_H
#define MSI_CARCASSONNE_IBOARD_H
#include "Tiles.h"

namespace carcassonne {

constexpr mb::size g_board_width = 41;
constexpr mb::size g_board_height = 41;
constexpr mb::size g_board_center_x = 20;
constexpr mb::size g_board_center_y = 20;

/*
 The maximal board size is 41x41
 Starting tile is at x = 20, y = 20
 */
class IBoard {
 public:
   [[nodiscard]] virtual int min_x() const noexcept = 0;
   [[nodiscard]] virtual int min_y() const noexcept = 0;
   [[nodiscard]] virtual int max_x() const noexcept = 0;
   [[nodiscard]] virtual int max_y() const noexcept = 0;
   [[nodiscard]] virtual TilePlacement tile_at(int x, int y) const noexcept = 0;
   [[nodiscard]] virtual bool can_place_at(int x, int y, TileType t, mb::u8 rotation) const noexcept = 0;
   virtual void set_tile(int x, int y, TileType t, mb::u8 rotation) noexcept = 0;
};

}

#endif//MSI_CARCASSONNE_IBOARD_H
