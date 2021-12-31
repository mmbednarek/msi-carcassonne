#ifndef CARCASSONNE_GPU_DIRECTION_H
#define CARCASSONNE_GPU_DIRECTION_H
#ifdef __CUDACC__
#define DEVHOST __device__ __host__
#else
#define DEVHOST
#endif

#include "Position.h"
#include <mb/int.h>
#include <tuple>

namespace carcassonne {

enum class Direction : mb::u8 {
   North = 0,
   East = 1,
   South = 2,
   West = 3,
   Middle = 4,
   NorthEast = 5,
   EastNorth = 6,
   EastSouth = 7,
   SouthEast = 8,
   SouthWest = 9,
   WestSouth = 10,
   WestNorth = 11,
   NorthWest = 12
};

DEVHOST constexpr std::array<Direction, 13> g_directions{
        Direction::North,
        Direction::East,
        Direction::South,
        Direction::West,
        Direction::Middle,
        Direction::NorthEast,
        Direction::EastNorth,
        Direction::SouthEast,
        Direction::EastSouth,
        Direction::SouthWest,
        Direction::WestSouth,
        Direction::NorthWest,
        Direction::WestNorth,
};

constexpr bool is_side_direction(Direction d) {
   switch (d) {
   case Direction::North:
   case Direction::East:
   case Direction::South:
   case Direction::West:
      return true;
   default:
      return false;
   }
}

constexpr std::tuple<double, double> direction_position(TilePosition tp, Direction d) {
   switch (d) {
   case Direction::North:
      return std::make_tuple(static_cast<double>(tp.x) + 0.5, static_cast<double>(tp.y));
   case Direction::East:
      return std::make_tuple(static_cast<double>(tp.x) + 1.0, static_cast<double>(tp.y) + 0.5);
   case Direction::South:
      return std::make_tuple(static_cast<double>(tp.x) + 0.5, static_cast<double>(tp.y) + 1.0);
   case Direction::West:
      return std::make_tuple(static_cast<double>(tp.x), static_cast<double>(tp.y) + 0.5);
   case Direction::Middle:
      return std::make_tuple(static_cast<double>(tp.x) + 0.5, static_cast<double>(tp.y) + 0.5);
   case Direction::EastNorth:
      return std::make_tuple(static_cast<double>(tp.x) + 1.0, static_cast<double>(tp.y) + 0.25);
   case Direction::NorthEast:
      return std::make_tuple(static_cast<double>(tp.x) + 0.75, static_cast<double>(tp.y));
   case Direction::SouthEast:
      return std::make_tuple(static_cast<double>(tp.x) + 0.75, static_cast<double>(tp.y) + 1.0);
   case Direction::EastSouth:
      return std::make_tuple(static_cast<double>(tp.x) + 1.0, static_cast<double>(tp.y) + 0.75);
   case Direction::SouthWest:
      return std::make_tuple(static_cast<double>(tp.x) + 0.25, static_cast<double>(tp.y) + 1.0);
   case Direction::WestSouth:
      return std::make_tuple(static_cast<double>(tp.x), static_cast<double>(tp.y) + 0.75);
   case Direction::NorthWest:
      return std::make_tuple(static_cast<double>(tp.x) + 0.25, static_cast<double>(tp.y));
   case Direction::WestNorth:
      return std::make_tuple(static_cast<double>(tp.x), static_cast<double>(tp.y) + 0.25);
   }
   return std::make_tuple(static_cast<double>(tp.x), static_cast<double>(tp.y));
}

}// namespace carcassonne

#endif//CARCASSONNE_GPU_DIRECTION_H
