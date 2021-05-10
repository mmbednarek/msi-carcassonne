#ifndef MSI_CARCASSONNE_CORE_H
#define MSI_CARCASSONNE_CORE_H
#include <algorithm>
#include <array>
#include <mb/int.h>
#include <mb/view.h>
#include <memory>

namespace carcassonne {

constexpr mb::size g_board_width = 141;
constexpr mb::size g_board_height = 141;

enum class EdgeType {
   Grass,
   Path,
   Town
};

enum class Connection : mb::u64 {
   None        = 0,
   NorthXX     = 1 << 0,
   NorthXY     = 1 << 1,
   NorthYX     = 1 << 2,
   NorthYY     = 1 << 3,
   North       = 0x000900000f,
   EastXX      = 1 << 4,
   EastXY      = 1 << 5,
   EastYX      = 1 << 6, 
   EastYY      = 1 << 7, 
   East        = 0x00030000f0,
   SouthXX     = 1 << 8, 
   SouthXY     = 1 << 9, 
   SouthYX     = 1 << 10,
   SouthYY     = 1 << 11,
   South       = 0x0006000f00,
   WestXX      = 1 << 12,
   WestXY      = 1 << 13,
   WestYX      = 1 << 14,
   WestYY      = 1 << 15,
   West        = 0x000c00f000,
   CrossWestXX = 1 << 16,
   CrossWestXY = 1 << 17,
   CrossWestYX = 1 << 18,
   CrossWestYY = 1 << 19,
   CrossWest   = 0x000a0f0000,
   CrossEastXX = 1 << 20,
   CrossEastXY = 1 << 21,
   CrossEastYX = 1 << 22,
   CrossEastYY = 1 << 23,
<<<<<<< HEAD
   CrossEast   = 0x0005f00000,
=======
   CrossEast   = 0x0000f00000,
>>>>>>> added field_edges
   NorthEastXY  = 1 << 24,
   SouthEastXY  = 1 << 25,
   SouthWestXY  = 1 << 26,
   NorthWestXY  = 1 << 27,
   AllFieldEdges  = 0x000fffffff,
   NorthEast   = 0x0100000000,
   NorthSouth  = 0x0200000000,
   NorthWest   = 0x0400000000,
   SouthEast   = 0x0800000000,
   SouthWest   = 0x1000000000,
   WestEast    = 0x2000000000,
   AllEdges    = 0x3f00000000,
   MiddleNorth = 0x04000000000,
   MiddleEast  = 0x08000000000,
   MiddleSouth = 0x10000000000,
   MiddleWest  = 0x20000000000,
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

   if (c & Connection::MiddleNorth)
      result |= Connection::MiddleEast;
   if (c & Connection::MiddleEast)
      result |= Connection::MiddleSouth;
   if (c & Connection::MiddleSouth)
      result |= Connection::MiddleWest;
   if (c & Connection::MiddleWest)
      result |= Connection::MiddleNorth;

   if (c & Connection::NorthXX)
      result |= Connection::EastYY;
   if (c & Connection::EastYY)
      result |= Connection::SouthXX;
   if (c & Connection::SouthXX)
      result |= Connection::WestYY;
   if (c & Connection::WestYY)
      result |= Connection::NorthXX;

   if (c & Connection::NorthXY)
      result |= Connection::EastYX;
   if (c & Connection::EastYX)
      result |= Connection::SouthYX;
   if (c & Connection::SouthYX)
      result |= Connection::WestXY;
   if (c & Connection::WestXY)
      result |= Connection::NorthXY;

   if (c & Connection::NorthYX)
      result |= Connection::EastXY;
   if (c & Connection::EastXY)
      result |= Connection::SouthXY;
   if (c & Connection::SouthXY)
      result |= Connection::WestYX;
   if (c & Connection::WestYX)
      result |= Connection::NorthYX;

   if (c & Connection::NorthYY)
      result |= Connection::EastXX;
   if (c & Connection::EastXX)
      result |= Connection::SouthYY;
   if (c & Connection::SouthYY)
      result |= Connection::WestXX;
   if (c & Connection::WestXX)
      result |= Connection::NorthYY;

   if (c & Connection::CrossWestXX)
      result |= Connection::CrossEastYY;
   if (c & Connection::CrossEastYY)
      result |= Connection::CrossWestXX;

   if (c & Connection::CrossWestXY)
      result |= Connection::CrossEastYX;
   if (c & Connection::CrossEastYX)
      result |= Connection::CrossWestXY;

   if (c & Connection::CrossWestYX)
      result |= Connection::CrossEastXY;
   if (c & Connection::CrossEastXY)
      result |= Connection::CrossWestYX;

   if (c & Connection::CrossWestYY)
      result |= Connection::CrossEastXX;
   if (c & Connection::CrossEastXX)
      result |= Connection::CrossWestYY;
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> added corners rotation list

   if (c & Connection::NorthEastXY)
      result |= Connection::SouthEastXY;
   if (c & Connection::SouthEastXY)
      result |= Connection::SouthWestXY;
   if (c & Connection::SouthWestXY)
      result |= Connection::NorthWestXY;
   if (c & Connection::NorthWestXY)
      result |= Connection::NorthEastXY;
<<<<<<< HEAD
=======
>>>>>>> added field_edges
=======
>>>>>>> added corners rotation list
   
   return result;
}

struct Tile {
   std::array<EdgeType, 4> edges{};
   std::array<EdgeType, 8> field_edges{};
   Connection connections = Connection::None;
   bool monastery = false;
   bool pennant = false;

   [[nodiscard]] inline Tile rotate(mb::size count) const {
      Connection rotated = connections;
      for (mb::size i = 0; i < count; ++i)
         rotated = rotate_connection(rotated);

      Tile result{
              .connections = rotated,
              .monastery = monastery,
              .pennant = pennant,
      };

      std::copy(edges.begin(), edges.end(), result.edges.begin());
      std::copy(field_edges.begin(), field_edges.end(), result.field_edges.begin());

      std::rotate(result.edges.begin(), result.edges.end() - count, result.edges.end());
      std::rotate(result.field_edges.begin(), result.field_edges.end() - count, result.field_edges.end());
<<<<<<< HEAD
      std::rotate(result.field_edges.begin(), result.field_edges.end() - count, result.field_edges.end());
=======
>>>>>>> added field_edges
      
      return result;
   }
};

constexpr std::array<Tile, 25> g_tiles{
        Tile{
                .edges{EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass},
                .field_edges{EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass},
        },
        {
                .edges{EdgeType::Path, EdgeType::Town, EdgeType::Path, EdgeType::Grass},
                .field_edges{EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass},
                .connections = Connection::NorthSouth | Connection::EastYY | Connection::West,
        },
        {
                .edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass},
                .field_edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Town},
                .connections = Connection::NorthXX | Connection::EastXX | Connection::EastXY | Connection::South | Connection::WestXX | Connection::WestXY | Connection::CrossWestXX | Connection::CrossWestXY | Connection::CrossEastXX | Connection::CrossEastXY,
        },
        {
                .edges{EdgeType::Town, EdgeType::Grass, EdgeType::Town, EdgeType::Grass},
                .field_edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Town},
                .connections = Connection::NorthSouth | Connection::EastXX | Connection::WestXX,
        },
        {
                .edges{EdgeType::Grass, EdgeType::Town, EdgeType::Grass, EdgeType::Town},
                .field_edges{EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Grass},
                .connections = Connection::NorthYY | Connection::SouthYY | Connection::CrossWestYY | Connection::CrossEastYY,
        },
        {
                .edges{EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass},
                .field_edges{EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass},
                .connections = Connection::AllFieldEdges,
                .monastery = true,
        },
        {
                .edges{EdgeType::Grass, EdgeType::Grass, EdgeType::Path, EdgeType::Grass},
                .field_edges{EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass},
                .connections = Connection::MiddleSouth | Connection::AllFieldEdges,
                .monastery = true,
        },
        {
                .edges{EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Grass},
                .field_edges{EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass},
                .connections = Connection::NorthXY | Connection::NorthYY | Connection::WestXX | Connection::WestYX | Connection::CrossEastYX,
        },
        {
                .edges{EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Town},
                .field_edges{EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Town},
                .connections = Connection::AllEdges,
                .pennant = true,
        },
        {
                .edges{EdgeType::Town, EdgeType::Path, EdgeType::Path, EdgeType::Grass},
                .field_edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Town},
                .connections = Connection::SouthEast | Connection::NorthXX | Connection::WestXX | Connection::WestXY | Connection::CrossEastXX | Connection::CrossEastXY | Connection::SouthEastXY | Connection::SouthWestXY,
        },
        {
                .edges{EdgeType::Town, EdgeType::Grass, EdgeType::Path, EdgeType::Path},
                .field_edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Town},
                .connections = Connection::SouthWest | Connection::NorthXX | Connection::EastXX | Connection::EastXY | Connection::CrossWestXX | Connection::CrossWestXY | Connection::SouthEastXY | Connection::SouthWestXY,
        },
        {
                .edges{EdgeType::Town, EdgeType::Path, EdgeType::Path, EdgeType::Path},
                .field_edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Town},
                .connections = Connection::NorthXX | Connection::MiddleEast | Connection::MiddleSouth | Connection::MiddleWest | Connection::SouthEastXY | Connection::SouthWestXY,
        },
        {
                .edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Town},
                .field_edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Town},
                .connections = Connection::NorthWest | Connection::SouthYX | Connection::SouthYY | Connection::EastXX | Connection::EastXY | Connection::CrossEastXY | Connection::SouthEastXY,
                .pennant = true,
        },
        {
                .edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Town},
<<<<<<< HEAD
                .field_edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Town},
=======
                .field_edges{EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Town, EdgeType::Town},
>>>>>>> added field_edges
                .connections = Connection::NorthWest | Connection::SouthYX | Connection::SouthYY | Connection::EastXX | Connection::EastXY | Connection::CrossEastXY | Connection::SouthEastXY,
        },
        {
                .edges{EdgeType::Town, EdgeType::Path, EdgeType::Path, EdgeType::Town},
<<<<<<< HEAD
                .field_edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Town},
=======
                .field_edges{EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Town, EdgeType::Town},
>>>>>>> added field_edges
                .connections = Connection::NorthWest | Connection::SouthEast | Connection::CrossEastXY | Connection::SouthEastXY,
        },
        {
                .edges{EdgeType::Town, EdgeType::Path, EdgeType::Path, EdgeType::Town},
<<<<<<< HEAD
                .field_edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Town},
=======
                .field_edges{EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Town, EdgeType::Town},
>>>>>>> added field_edges
                .connections = Connection::NorthWest | Connection::SouthEast | Connection::CrossEastXY | Connection::SouthEastXY,
                .pennant = true,
        },
        {
                .edges{EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Town},
                .field_edges{EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Town},
                .connections = Connection::NorthEast | Connection::NorthWest | Connection::WestEast | Connection::SouthYY,
                .pennant = true,
        },
        {
                .edges{EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Town},
                .field_edges{EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Town},
                .connections = Connection::NorthEast | Connection::NorthWest | Connection::WestEast | Connection::SouthYY,
        },
        {
                .edges{EdgeType::Town, EdgeType::Town, EdgeType::Path, EdgeType::Town},
                .field_edges{EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Town},
                .connections = Connection::NorthEast | Connection::NorthWest | Connection::WestEast | Connection::MiddleSouth,
                .pennant = true,
        },
        {
                .edges{EdgeType::Town, EdgeType::Town, EdgeType::Path, EdgeType::Town},
                .field_edges{EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Town},
                .connections = Connection::NorthEast | Connection::NorthWest | Connection::WestEast | Connection::MiddleSouth,
        },
        {
                .edges{EdgeType::Path, EdgeType::Grass, EdgeType::Path, EdgeType::Grass},
                .field_edges{EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass},
                .connections = Connection::NorthSouth | Connection::West | Connection::East | Connection::CrossEastXY,
        },
        {
                .edges{EdgeType::Grass, EdgeType::Grass, EdgeType::Path, EdgeType::Path},
                .field_edges{EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass},
                .connections = Connection::SouthWest | Connection::North | Connection::East | Connection::CrossWest | Connection::SouthWestXY,
        },
        {
                .edges{EdgeType::Grass, EdgeType::Path, EdgeType::Path, EdgeType::Path},
                .field_edges{EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass},
                .connections = Connection::North | Connection::MiddleEast | Connection::MiddleSouth | Connection::MiddleWest | Connection::SouthEastXY | Connection::SouthWestXY,
        },
        {
                .edges{EdgeType::Path, EdgeType::Path, EdgeType::Path, EdgeType::Path},
                .field_edges{EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass},
                .connections = Connection::MiddleNorth | Connection::MiddleEast | Connection::MiddleSouth | Connection::MiddleWest | Connection::NorthEastXY | Connection::SouthEastXY | Connection::SouthWestXY | Connection::NorthWestXY,
        },
        {
                .edges{EdgeType::Town, EdgeType::Grass, EdgeType::Town, EdgeType::Grass},
                .field_edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Town},
                .connections = Connection::NorthSouth | Connection::WestXX | Connection::EastXX,
                .pennant = true,
        },
};

using TileType = mb::u8;

struct TilePlacement {
   TileType type = 0;
   mb::u8 rotation = 0;

   [[nodiscard]] inline Tile tile() const {
      return g_tiles[type].rotate(rotation);
   }
};

struct TilePosition {
   int x, y;
};


enum class Direction {
   North,
   East,
   South,
   West,
   Middle,
   NorthEastX,
   NorthEastY,
   SouthEastX,
   SouthEastY,
   SouthWestX,
   SouthWestY,
   NorthWestX,
   NorthWestY
};

constexpr std::array<Direction, 13> g_directions{
        Direction::North,
        Direction::East,
        Direction::South,
        Direction::West,
        Direction::Middle,
        Direction::NorthEastX,
        Direction::NorthEastY,
        Direction::SouthEastX,
        Direction::SouthEastY,
        Direction::SouthWestX,
        Direction::SouthWestY,
        Direction::NorthWestX,
        Direction::NorthWestY
};

constexpr std::tuple<double, double> direction_position(TilePosition tp, Direction d) {
   switch (d) {
   case Direction::North:
      return std::make_tuple(static_cast<double>(tp.x) + 0.5, static_cast<double>(tp.y) + 0.1);
   case Direction::East:
      return std::make_tuple(static_cast<double>(tp.x) + 0.9, static_cast<double>(tp.y) + 0.5);
   case Direction::South:
      return std::make_tuple(static_cast<double>(tp.x) + 0.5, static_cast<double>(tp.y) + 0.9);
   case Direction::West:
      return std::make_tuple(static_cast<double>(tp.x) + 0.1, static_cast<double>(tp.y) + 0.5);
   case Direction::Middle:
      return std::make_tuple(static_cast<double>(tp.x) + 0.5, static_cast<double>(tp.y) + 0.5);
   case Direction::NorthEastY:
      return std::make_tuple(static_cast<double>(tp.x) + 0.8, static_cast<double>(tp.y) + 0.1);
   case Direction::NorthEastX:
      return std::make_tuple(static_cast<double>(tp.x) + 0.9, static_cast<double>(tp.y) + 0.2);
   case Direction::SouthEastX:
      return std::make_tuple(static_cast<double>(tp.x) + 0.9, static_cast<double>(tp.y) + 0.8);
   case Direction::SouthEastY:
      return std::make_tuple(static_cast<double>(tp.x) + 0.8, static_cast<double>(tp.y) + 0.9);
   case Direction::SouthWestX:
      return std::make_tuple(static_cast<double>(tp.x) + 0.1, static_cast<double>(tp.y) + 0.8);
   case Direction::SouthWestY:
      return std::make_tuple(static_cast<double>(tp.x) + 0.2, static_cast<double>(tp.y) + 0.9);
   case Direction::NorthWestX:
      return std::make_tuple(static_cast<double>(tp.x) + 0.1, static_cast<double>(tp.y) + 0.2);
   case Direction::NorthWestY:
      return std::make_tuple(static_cast<double>(tp.x) + 0.2, static_cast<double>(tp.y) + 0.1);
   }
   return std::make_tuple(static_cast<double>(tp.x), static_cast<double>(tp.y));
}


[[nodiscard]] constexpr std::tuple<Connection, Direction, Direction> read_directions(Connection c) {
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

   if (c & Connection::MiddleNorth)
      return std::make_tuple(c - Connection::MiddleNorth, Direction::Middle, Direction::North);
   if (c & Connection::MiddleEast)
      return std::make_tuple(c - Connection::MiddleEast, Direction::Middle, Direction::East);
   if (c & Connection::MiddleSouth)
      return std::make_tuple(c - Connection::MiddleSouth, Direction::Middle, Direction::South);
   if (c & Connection::MiddleWest)
      return std::make_tuple(c - Connection::MiddleWest, Direction::Middle, Direction::West);

   if (c & Connection::NorthXX)
      return std::make_tuple(c - Connection::NorthXX, Direction::NorthWestX, Direction::NorthEastX);
   if (c & Connection::NorthXY)
      return std::make_tuple(c - Connection::NorthXY, Direction::NorthWestX, Direction::NorthEastY);
   if (c & Connection::NorthYX)
      return std::make_tuple(c - Connection::NorthYX, Direction::NorthWestY, Direction::NorthEastX);
   if (c & Connection::NorthYY)
      return std::make_tuple(c - Connection::NorthYY, Direction::NorthWestY, Direction::NorthEastY);

   if (c & Connection::EastXX)
      return std::make_tuple(c - Connection::EastXX, Direction::NorthEastX, Direction::SouthEastX);
   if (c & Connection::EastXY)
      return std::make_tuple(c - Connection::EastXY, Direction::NorthEastX, Direction::SouthEastY);
   if (c & Connection::EastYX)
      return std::make_tuple(c - Connection::EastYX, Direction::NorthEastY, Direction::SouthEastX);
   if (c & Connection::EastYY)
      return std::make_tuple(c - Connection::EastYY, Direction::NorthEastY, Direction::SouthEastY);

   if (c & Connection::SouthXX)
      return std::make_tuple(c - Connection::SouthXX, Direction::SouthWestX, Direction::SouthEastX);
   if (c & Connection::SouthXY)
      return std::make_tuple(c - Connection::SouthXY, Direction::SouthWestX, Direction::SouthEastY);
   if (c & Connection::SouthYX)
      return std::make_tuple(c - Connection::SouthYX, Direction::SouthWestY, Direction::SouthEastX);
   if (c & Connection::SouthYY)
      return std::make_tuple(c - Connection::SouthYY, Direction::SouthWestY, Direction::SouthEastY);

   if (c & Connection::WestXX)
      return std::make_tuple(c - Connection::WestXX, Direction::NorthWestX, Direction::SouthWestX);
   if (c & Connection::WestXY)
      return std::make_tuple(c - Connection::WestXY, Direction::NorthWestX, Direction::SouthWestY);
   if (c & Connection::WestYX)
      return std::make_tuple(c - Connection::WestYX, Direction::NorthWestY, Direction::SouthWestX);
   if (c & Connection::WestYY)
      return std::make_tuple(c - Connection::WestYY, Direction::NorthWestY, Direction::SouthWestY);

   if (c & Connection::CrossWestXX)
      return std::make_tuple(c - Connection::CrossWestXX, Direction::NorthWestX, Direction::SouthEastX);
   if (c & Connection::CrossWestXY)
      return std::make_tuple(c - Connection::CrossWestXY, Direction::NorthWestX, Direction::SouthEastY);
   if (c & Connection::CrossWestYX)
      return std::make_tuple(c - Connection::CrossWestYX, Direction::NorthWestY, Direction::SouthEastX);
   if (c & Connection::CrossWestYY)
      return std::make_tuple(c - Connection::CrossWestYY, Direction::NorthWestY, Direction::SouthEastY);

   if (c & Connection::CrossEastXX)
      return std::make_tuple(c - Connection::CrossEastXX, Direction::NorthEastX, Direction::SouthWestX);
   if (c & Connection::CrossEastXY)
      return std::make_tuple(c - Connection::CrossEastXY, Direction::NorthEastX, Direction::SouthWestY);
   if (c & Connection::CrossEastYX)
      return std::make_tuple(c - Connection::CrossEastYX, Direction::NorthEastY, Direction::SouthWestX);
   if (c & Connection::CrossEastYY)
      return std::make_tuple(c - Connection::CrossEastYY, Direction::NorthEastY, Direction::SouthWestY);

   if (c & Connection::NorthEastXY)
      return std::make_tuple(c - Connection::NorthEastXY, Direction::NorthEastX, Direction::NorthEastY);
   if (c & Connection::SouthEastXY)
<<<<<<< HEAD
      return std::make_tuple(c - Connection::SouthEastXY, Direction::SouthEastX, Direction::SouthEastY);
=======
      return std::make_tuple(c - Connection::SouthEastXY, Direction::SouthEastY, Direction::SouthEastY);
>>>>>>> added field_edges
   if (c & Connection::SouthWestXY)
      return std::make_tuple(c - Connection::SouthWestXY, Direction::SouthWestX, Direction::SouthWestY);
   if (c & Connection::NorthWestXY)
      return std::make_tuple(c - Connection::NorthWestXY, Direction::NorthWestX, Direction::NorthWestY);
   
   return std::make_tuple(Connection::None, Direction::Middle, Direction::Middle);
}

using Edge = int;

<<<<<<< HEAD
constexpr auto g_west_east_edges = g_board_width * (g_board_height + 1);
constexpr auto g_middle_edges = g_west_east_edges + (g_board_width + 1) * g_board_height;
constexpr auto g_east_edges = g_middle_edges + g_board_width * g_board_height;
constexpr auto g_west_edges = g_east_edges + g_board_width * (g_board_height + 1);
constexpr auto g_north_edges = g_west_edges + g_board_width * (g_board_height + 1);
constexpr auto g_south_edges = g_north_edges + (g_board_width + 1) * g_board_height;
=======
constexpr auto g_west_east_edges = g_board_width + (g_board_height + 1);
constexpr auto g_middle_edges = g_west_east_edges + (g_board_width + 1) + g_board_height;
constexpr auto g_east_edges = g_middle_edges + g_board_width * g_board_height;
constexpr auto g_west_edges = g_east_edges + g_board_width + (g_board_height + 1);
constexpr auto g_north_edges = g_west_edges + g_board_width + (g_board_height + 1);
<<<<<<< HEAD
constexpr auto g_south_edges = g_west_edges + (g_board_width + 1) + g_board_height;
>>>>>>> added field_edges
=======
constexpr auto g_south_edges = g_north_edges + (g_board_width + 1) + g_board_height;
>>>>>>> added corners rotation list

constexpr Edge make_edge(int x, int y, Direction d) {
   switch (d) {
   case Direction::North:
      return static_cast<Edge>(x + y * g_board_width);
   case Direction::South:
      return static_cast<Edge>(x + (y + 1) * g_board_width);

   case Direction::West:
      return static_cast<Edge>(g_west_east_edges + y + x * (g_board_height));
   case Direction::East:
      return static_cast<Edge>(g_west_east_edges + y + (x + 1) * (g_board_height));

   case Direction::Middle:
      return static_cast<Edge>(g_middle_edges + x + y * g_board_height);
      
   case Direction::NorthEastY:
      return static_cast<Edge>(g_east_edges + x + y * g_board_width);
   case Direction::SouthEastY:
      return static_cast<Edge>(g_east_edges + x + (y + 1) * g_board_width);
      
   case Direction::NorthWestY:
      return static_cast<Edge>(g_west_edges + x + y * g_board_width);
   case Direction::SouthWestY:
      return static_cast<Edge>(g_west_edges + x + (y + 1) * g_board_width);
      
   case Direction::NorthWestX:
      return static_cast<Edge>(g_north_edges + y + x * (g_board_height));
   case Direction::NorthEastX:
      return static_cast<Edge>(g_north_edges + y + (x + 1) * (g_board_height));
      
   case Direction::SouthWestX:
      return static_cast<Edge>(g_south_edges + y + x * (g_board_height));
   case Direction::SouthEastX:
      return static_cast<Edge>(g_south_edges + y + (x + 1) * (g_board_height));
   }
}

enum class Player : mb::u8 {
   Black = 1 << 0,
   Blue = 1 << 1,
   Yellow = 1 << 2,
   Red = 1 << 3,
};

[[nodiscard]] constexpr int player_to_int(Player p) {
   switch (p) {
   case Player::Black:
      return 0;
   case Player::Blue:
      return 1;
   case Player::Yellow:
      return 2;
   case Player::Red:
      return 3;
   }
   return 0;
}

struct Figure {
   Player player;
   double x, y;
};

constexpr Player next_player(Player player, mb::u8 player_count) {
   auto current_player = static_cast<mb::u8>(player);
   current_player <<= 1;
   if (current_player >= (1 << player_count))
      current_player = 1;
   return static_cast<Player>(current_player);
}

[[nodiscard]] constexpr Player operator|(Player left, Player right) {
   return static_cast<Player>(static_cast<mb::u32>(left) | static_cast<mb::u32>(right));
}

constexpr Player &operator|=(Player &left, Player right) {
   left = left | right;
   return left;
}

[[nodiscard]] constexpr bool operator&(Player left, Player right) {
   return (static_cast<mb::u32>(left) & static_cast<mb::u32>(right)) != 0;
}

enum class MovePhase {
   PlaceTile,
   PlaceFigure,
   Done
};

class IMove {
 public:
   [[nodiscard]] virtual Player player() const noexcept = 0;
   [[nodiscard]] virtual TileType tile_type() const noexcept = 0;
   [[nodiscard]] virtual MovePhase phase() const noexcept = 0;
   [[nodiscard]] virtual bool is_free(Direction d) const noexcept = 0;
   [[nodiscard]] virtual TilePosition position() const noexcept = 0;
   virtual void place_tile(int x, int y, mb::u8 rotation) noexcept = 0;
   virtual void place_figure(Direction d) noexcept = 0;
   virtual void ignore_figure() noexcept = 0;
};

/*
 The maximal board size is 141x141
 Starting tile is at x = 70, y = 70
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

class IGame {
 public:
   [[nodiscard]] virtual const IBoard &board() const noexcept = 0;
   [[nodiscard]] virtual Player current_player() const noexcept = 0;
   [[nodiscard]] virtual std::unique_ptr<IMove> new_move(Player p) noexcept = 0;
   [[nodiscard]] virtual mb::view<Figure> figures() const noexcept = 0;
};

}// namespace carcassonne

#endif//MSI_CARCASSONNE_CORE_H