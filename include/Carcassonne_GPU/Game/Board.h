#ifndef CARCASSONNE_GPU_GAME_BOARD_H
#define CARCASSONNE_GPU_GAME_BOARD_H
#ifdef __CUDACC__
#define DEVHOST __device__ __host__
#else
#define DEVHOST
#endif

#include <array>
#include <mb/int.h>
#include <Util_GPU/mb/vector2d.h>
#include <Carcassonne_GPU/Tiles.h>

namespace carcassonne {

constexpr mb::size g_board_width = 41;
constexpr mb::size g_board_height = 41;
constexpr mb::size g_board_center_x = 20;
constexpr mb::size g_board_center_y = 20;

}// namespace carcassonne

/*
 The maximal board size is 41x41
 Starting tile is at x = 20, y = 20
 */
namespace carcassonne::game {

class Board {
   mb::vector2d<TilePlacement> m_board;
   int m_min_x = 20;
   int m_min_y = 20;
   int m_max_x = 21;
   int m_max_y = 21;

 public:
   [[nodiscard]] int min_x() const noexcept;
   [[nodiscard]] int min_y() const noexcept;
   [[nodiscard]] int max_x() const noexcept;
   [[nodiscard]] int max_y() const noexcept;
   [[nodiscard]] TilePlacement tile_at(int x, int y) const noexcept;
   [[nodiscard]] TilePlacement tile_at(TilePosition pos) const noexcept;
   [[nodiscard]] bool can_place_at(int x, int y, TileType t, mb::u8 rotation) const noexcept;
   void set_tile(int x, int y, TileType t, mb::u8 rotation) noexcept;
};

}// namespace carcassonne::game

#endif//CARCASSONNE_GPU_GAME_BOARD_H
