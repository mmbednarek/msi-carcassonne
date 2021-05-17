#ifndef MSI_CARCASSONNE_DIRECTION_H
#define MSI_CARCASSONNE_DIRECTION_H

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

constexpr std::array<Direction, 13> g_directions{
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

}// namespace carcassonne

#endif//MSI_CARCASSONNE_DIRECTION_H
