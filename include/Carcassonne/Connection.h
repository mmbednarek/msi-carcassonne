#ifndef MSI_CARCASSONNE_CONNECTION_H
#define MSI_CARCASSONNE_CONNECTION_H
#include "Direction.h"
#include <mb/int.h>

namespace carcassonne {

enum class Connection : mb::u64 {
   None = 0,
   NorthX = 1 << 0,
   NorthY = 1 << 1,
   EastX = 1 << 2,
   EastY = 1 << 3,
   SouthX = 1 << 4,
   SouthY = 1 << 5,
   WestX = 1 << 6,
   WestY = 1 << 7,
   NorthEastCorner = 1 << 8,
   SouthEastCorner = 1 << 9,
   SouthWestCorner = 1 << 10,
   NorthWestCorner = 1 << 11,
   Corners = NorthEastCorner | SouthEastCorner | SouthWestCorner | NorthWestCorner,
   North = NorthX | NorthY | NorthWestCorner | NorthEastCorner,
   East = EastX | EastY | NorthEastCorner | SouthEastCorner,
   South = SouthX | SouthY | SouthEastCorner | SouthWestCorner,
   West = WestX | WestY | SouthWestCorner | NorthWestCorner,
   EastNorthCross = 1 << 12,
   SouthEastCross = 1 << 13,
   WestSouthCross = 1 << 14,
   NorthWestCross = 1 << 15,
   AllFieldEdges = North | East | South | West,
   NorthEast = 1 << 16,
   NorthSouth = 1 << 17,
   NorthWest = 1 << 18,
   SouthEast = 1 << 19,
   SouthWest = 1 << 20,
   WestEast = 1 << 21,
   AllEdges = NorthEast | NorthSouth | NorthWest | SouthEast | SouthWest | WestEast,
};

[[nodiscard]] constexpr Connection operator|(Connection left, Connection right) {
   return static_cast<Connection>(static_cast<mb::u64>(left) | static_cast<mb::u64>(right));
}

constexpr Connection &operator|=(Connection &left, Connection right) {
   left = left | right;
   return left;
}

[[nodiscard]] constexpr bool operator&(Connection left, Connection right) {
   return (static_cast<mb::u64>(left) & static_cast<mb::u64>(right)) != 0;
}

[[nodiscard]] constexpr Connection operator-(Connection left, Connection right) {
   return static_cast<Connection>(static_cast<mb::u64>(left) & (~static_cast<mb::u64>(right)));
}

[[nodiscard]] constexpr Connection rotate_connection(Connection c) {
   Connection result{};
   if (c & Connection::NorthEast)
      result |= Connection::SouthEast;
   if (c & Connection::SouthEast)
      result |= Connection::SouthWest;
   if (c & Connection::SouthWest)
      result |= Connection::NorthWest;
   if (c & Connection::NorthWest)
      result |= Connection::NorthEast;

   if (c & Connection::NorthSouth)
      result |= Connection::WestEast;
   if (c & Connection::WestEast)
      result |= Connection::NorthSouth;

   if (c & Connection::NorthX)
      result |= Connection::EastY;
   if (c & Connection::EastY)
      result |= Connection::SouthX;
   if (c & Connection::SouthX)
      result |= Connection::WestY;
   if (c & Connection::WestY)
      result |= Connection::NorthX;

   if (c & Connection::NorthY)
      result |= Connection::EastX;
   if (c & Connection::EastX)
      result |= Connection::SouthY;
   if (c & Connection::SouthY)
      result |= Connection::WestX;
   if (c & Connection::WestX)
      result |= Connection::NorthY;

   if (c & Connection::NorthEastCorner)
      result |= Connection::SouthEastCorner;
   if (c & Connection::SouthEastCorner)
      result |= Connection::SouthWestCorner;
   if (c & Connection::SouthWestCorner)
      result |= Connection::NorthWestCorner;
   if (c & Connection::NorthWestCorner)
      result |= Connection::NorthEastCorner;

   if (c & Connection::EastNorthCross)
      result |= Connection::SouthEastCross;
   if (c & Connection::SouthEastCross)
      result |= Connection::WestSouthCross;
   if (c & Connection::WestSouthCross)
      result |= Connection::NorthWestCross;
   if (c & Connection::NorthWestCross)
      result |= Connection::EastNorthCross;

   return result;
}

[[nodiscard]] constexpr std::tuple<Connection, Direction, Direction> read_connections(Connection c) {
   if (c & Connection::NorthEast)
      return std::make_tuple(c - Connection::NorthEast, Direction::North, Direction::East);
   if (c & Connection::NorthSouth)
      return std::make_tuple(c - Connection::NorthSouth, Direction::North, Direction::South);
   if (c & Connection::NorthWest)
      return std::make_tuple(c - Connection::NorthWest, Direction::North, Direction::West);
   if (c & Connection::SouthEast)
      return std::make_tuple(c - Connection::SouthEast, Direction::South, Direction::East);
   if (c & Connection::SouthWest)
      return std::make_tuple(c - Connection::SouthWest, Direction::South, Direction::West);
   if (c & Connection::WestEast)
      return std::make_tuple(c - Connection::WestEast, Direction::West, Direction::East);

   if (c & Connection::NorthX)
      return std::make_tuple(c - Connection::NorthX, Direction::NorthWest, Direction::NorthEast);
   if (c & Connection::NorthY)
      return std::make_tuple(c - Connection::NorthY, Direction::WestNorth, Direction::EastNorth);

   if (c & Connection::EastX)
      return std::make_tuple(c - Connection::EastX, Direction::NorthEast, Direction::SouthEast);
   if (c & Connection::EastY)
      return std::make_tuple(c - Connection::EastY, Direction::EastNorth, Direction::EastSouth);

   if (c & Connection::SouthX)
      return std::make_tuple(c - Connection::SouthX, Direction::SouthWest, Direction::SouthEast);
   if (c & Connection::SouthY)
      return std::make_tuple(c - Connection::SouthY, Direction::WestSouth, Direction::EastSouth);

   if (c & Connection::WestX)
      return std::make_tuple(c - Connection::WestX, Direction::NorthWest, Direction::SouthWest);
   if (c & Connection::WestY)
      return std::make_tuple(c - Connection::WestY, Direction::WestNorth, Direction::WestSouth);

   if (c & Connection::NorthEastCorner)
      return std::make_tuple(c - Connection::NorthEastCorner, Direction::NorthEast, Direction::EastNorth);
   if (c & Connection::SouthEastCorner)
      return std::make_tuple(c - Connection::SouthEastCorner, Direction::SouthEast, Direction::EastSouth);
   if (c & Connection::SouthWestCorner)
      return std::make_tuple(c - Connection::SouthWestCorner, Direction::SouthWest, Direction::WestSouth);
   if (c & Connection::NorthWestCorner)
      return std::make_tuple(c - Connection::NorthWestCorner, Direction::NorthWest, Direction::WestNorth);

   if (c & Connection::EastNorthCross)
      return std::make_tuple(c - Connection::EastNorthCross, Direction::EastNorth, Direction::SouthWest);
   if (c & Connection::SouthEastCross)
      return std::make_tuple(c - Connection::SouthEastCross, Direction::SouthEast, Direction::WestNorth);
   if (c & Connection::WestSouthCross)
      return std::make_tuple(c - Connection::WestSouthCross, Direction::WestSouth, Direction::NorthEast);
   if (c & Connection::NorthWestCross)
      return std::make_tuple(c - Connection::NorthWestCross, Direction::NorthWest, Direction::EastSouth);

   return std::make_tuple(Connection::None, Direction::Middle, Direction::Middle);
}

}// namespace carcassonne

#endif//MSI_CARCASSONNE_CONNECTION_H
