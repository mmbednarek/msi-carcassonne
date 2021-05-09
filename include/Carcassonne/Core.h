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

enum class CornerType {
   Grass,
   Town
};

enum class Connection : mb::u32 {
   None = 0,
   NorthXX = 1 << 0,
   NorthXY = 1 << 1,
   NorthYX = 1 << 2,
   NorthYY = 1 << 3,
   North = 0x0000000f,
   EastXX = 1 << 4,
   EastXY = 1 << 5,
   EastYX = 1 << 6, 
   EastYY = 1 << 7, 
   East = 0x000000f0,
   SouthXX = 1 << 8, 
   SouthXY = 1 << 9, 
   SouthYX = 1 << 10,
   SouthYY = 1 << 11,
   South  = 0x00000f00,
   WestXX = 1 << 12,
   WestXY = 1 << 13,
   WestYX = 1 << 14,
   WestYY = 1 << 15,
   West = 0x0000f000,
   CrossWestXX = 1 << 16,
   CrossWestXY = 1 << 17,
   CrossWestYX = 1 << 18,
   CrossWestYY = 1 << 19,
   CrossWest = 0x000f0000,
   CrossEastXX = 1 << 20,
   CrossEastXY = 1 << 21,
   CrossEastYX = 1 << 22,
   CrossEastYY = 1 << 23,
   CrossEast = 0x00f00000,
   NorthXYiX = 0x00000005,
   NorthXYiY = 0x0000000a,
   NorthXiXY = 0x00000003,
   NorthYiXY = 0x0000000c,
   EastXYiX = 0x00000050,
   EastXYiY = 0x000000a0,
   EastXiXY = 0x00000030,
   EastYiXY = 0x000000c0,
   SouthXYiX = 0x00000500,
   SouthXYiY = 0x00000a00,
   SouthXiXY = 0x00000300,
   SouthYiXY = 0x00000c00,
   WestXYiX = 0x00005000,
   WestXYiY = 0x0000a000,
   WestXiXY = 0x00003000,
   WestYiXY = 0x0000c000,
   CrossWestXYiX = 0x0050000,
   CrossWestXYiY = 0x00a0000,
   CrossWestXiXY = 0x0030000,
   CrossWestYiXY = 0x00c0000,
   CrossEastXYiX = 0x00500000,
   CrossEastXYiY = 0x00a00000,
   CrossEastXiXY = 0x00300000,
   CrossEastYiXY = 0x00c00000,
   AllCorners = 0x00ffffff,
   NorthEast  = 1 << 24,
   NorthSouth = 1 << 25,
   NorthWest  = 1 << 26,
   SouthEast  = 1 << 27,
   SouthWest  = 1 << 28,
   WestEast   = 1 << 29,
   AllEdges   = 0x3f000000,
};

[[nodiscard]] constexpr Connection operator|(Connection left, Connection right) {
   return static_cast<Connection>(static_cast<mb::u32>(left) | static_cast<mb::u32>(right));
}

constexpr Connection &operator|=(Connection &left, Connection right) {
   left = left | right;
   return left;
}

[[nodiscard]] constexpr bool operator&(Connection left, Connection right) {
   return (static_cast<mb::u32>(left) & static_cast<mb::u32>(right)) != 0;
}

[[nodiscard]] constexpr Connection operator-(Connection left, Connection right) {
   return static_cast<Connection>(static_cast<mb::u32>(left) & (~static_cast<mb::u32>(right)));
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

   if (c & Connection::NorthXYiX)
      result |= Connection::EastXYiY;
   if (c & Connection::EastXYiY)
      result |= Connection::SouthXiXY;
   if (c & Connection::SouthXiXY)
      result |= Connection::WestYiXY;
   if (c & Connection::WestYiXY)
      result |= Connection::NorthXYiX;

   if (c & Connection::NorthXYiY)
      result |= Connection::EastXYiX;
   if (c & Connection::EastXYiX)
      result |= Connection::SouthYiXY;
   if (c & Connection::SouthYiXY)
      result |= Connection::WestXiXY;
   if (c & Connection::WestXiXY)
      result |= Connection::NorthXYiY;

   if (c & Connection::NorthXiXY)
      result |= Connection::EastYiXY;
   if (c & Connection::EastYiXY)
      result |= Connection::SouthXYiX;
   if (c & Connection::SouthXYiX)
      result |= Connection::WestXYiY;
   if (c & Connection::WestXYiY)
      result |= Connection::NorthXiXY;

   if (c & Connection::NorthYiXY)
      result |= Connection::EastXiXY;
   if (c & Connection::EastXiXY)
      result |= Connection::SouthXYiY;
   if (c & Connection::SouthXYiY)
      result |= Connection::WestXYiX;
   if (c & Connection::WestXYiX)
      result |= Connection::NorthYiXY;

   if (c & Connection::CrossWestXYiX)
      result |= Connection::CrossEastXYiY;
   if (c & Connection::CrossEastXYiY)
      result |= Connection::CrossWestXiXY;
   if (c & Connection::CrossWestXiXY)
      result |= Connection::CrossEastYiXY;
   if (c & Connection::CrossEastYiXY)
      result |= Connection::CrossWestXYiX;

   if (c & Connection::CrossWestXYiY)
      result |= Connection::CrossEastXYiX;
   if (c & Connection::CrossEastXYiX)
      result |= Connection::CrossWestYiXY;
   if (c & Connection::CrossWestYiXY)
      result |= Connection::CrossEastXiXY;
   if (c & Connection::CrossEastXiXY)
      result |= Connection::CrossWestXYiY;
   
   return result;
}

struct Tile {
   std::array<EdgeType, 4> edges{};
   std::array<CornerType, 4> corners{};
   Connection connections = Connection::None;
   bool monastery = false;
   bool pennant = false;

   [[nodiscard]] constexpr Tile rotate(mb::size count) const {
      Connection rotated = connections;
      for (mb::size i = 0; i < count; ++i)
         rotated = rotate_connection(rotated);

      Tile result{
              .connections = rotated,
              .monastery = monastery,
              .pennant = pennant,
      };

      std::copy(edges.begin(), edges.end(), result.edges.begin());
      std::copy(corners.begin(), corners.end(), result.corners.begin());

      std::rotate(result.edges.begin(), result.edges.end() - count, result.edges.end());
      return result;
      std::rotate(result.corners.begin(), result.corners.end() - count, result.corners.end());
      return result;
   }
};

constexpr std::array<Tile, 25> g_tiles{
        Tile{
                .edges{EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass},
                .corners{CornerType::Grass, CornerType::Grass, CornerType::Grass, CornerType::Grass},
        },
        {
                .edges{EdgeType::Path, EdgeType::Town, EdgeType::Path, EdgeType::Grass},
                .corners{CornerType::Grass, CornerType::Grass, CornerType::Grass, CornerType::Grass},
                .connections = Connection::NorthSouth | Connection::EastYY | Connection::West,
        },
        {
                .edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass},
                .corners{CornerType::Grass, CornerType::Grass, CornerType::Grass, CornerType::Grass},
                .connections = Connection::NorthXX | Connection::EastXiXY | Connection::South | Connection::WestXiXY | Connection::CrossWestXiXY | Connection::CrossEastXiXY,
        },
        {
                .edges{EdgeType::Town, EdgeType::Grass, EdgeType::Town, EdgeType::Grass},
                .corners{CornerType::Grass, CornerType::Grass, CornerType::Grass, CornerType::Grass},
                .connections = Connection::NorthSouth | Connection::EastXX | Connection::WestXX,
        },
        {
                .edges{EdgeType::Grass, EdgeType::Town, EdgeType::Grass, EdgeType::Town},
                .corners{CornerType::Grass, CornerType::Grass, CornerType::Grass, CornerType::Grass},
                .connections = Connection::NorthYY | Connection::SouthYY | Connection::CrossWestYY | Connection::CrossEastYY,
        },
        {
                .edges{EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass},
                .corners{CornerType::Grass, CornerType::Grass, CornerType::Grass, CornerType::Grass},
                .connections = Connection::AllCorners,
                .monastery = true,
        },
        {
                .edges{EdgeType::Grass, EdgeType::Grass, EdgeType::Path, EdgeType::Grass},
                .corners{CornerType::Grass, CornerType::Grass, CornerType::Grass, CornerType::Grass},
                .connections = Connection::AllCorners,
                .monastery = true,
        },
        {
                .edges{EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Grass},
                .corners{CornerType::Grass, CornerType::Town, CornerType::Grass, CornerType::Grass},
                .connections = Connection::NorthXYiY | Connection::WestXYiX | Connection::CrossEastYX,
        },
        {
                .edges{EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Town},
                .corners{CornerType::Town, CornerType::Town, CornerType::Town, CornerType::Town},
                .connections = Connection::AllEdges,
                .pennant = true,
        },
        {
                .edges{EdgeType::Town, EdgeType::Path, EdgeType::Path, EdgeType::Grass},
                .corners{CornerType::Grass, CornerType::Grass, CornerType::Grass, CornerType::Grass},
                .connections = Connection::SouthEast | Connection::WestXiXY | Connection::CrossEastXiXY,
        },
        {
                .edges{EdgeType::Town, EdgeType::Grass, EdgeType::Path, EdgeType::Path},
                .corners{CornerType::Grass, CornerType::Grass, CornerType::Grass, CornerType::Grass},
                .connections = Connection::SouthWest | Connection::EastXiXY | Connection::CrossWestXiXY,
        },
        {
                .edges{EdgeType::Town, EdgeType::Path, EdgeType::Path, EdgeType::Path},
                .corners{CornerType::Grass, CornerType::Grass, CornerType::Grass, CornerType::Grass},
                .connections = Connection::NorthXX,
        },
        {
                .edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Town},
                .corners{CornerType::Grass, CornerType::Grass, CornerType::Grass, CornerType::Town},
                .connections = Connection::NorthWest | Connection::SouthYiXY | Connection::EastXiXY | Connection::CrossEastXY,
                .pennant = true,
        },
        {
                .edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Town},
                .corners{CornerType::Grass, CornerType::Grass, CornerType::Grass, CornerType::Town},
                .connections = Connection::NorthWest | Connection::SouthYiXY | Connection::EastXiXY | Connection::CrossEastXY,
        },
        {
                .edges{EdgeType::Town, EdgeType::Path, EdgeType::Path, EdgeType::Town},
                .corners{CornerType::Grass, CornerType::Grass, CornerType::Grass, CornerType::Town},
                .connections = Connection::NorthWest | Connection::SouthEast | Connection::CrossEastXY,
        },
        {
                .edges{EdgeType::Town, EdgeType::Path, EdgeType::Path, EdgeType::Town},
                .corners{CornerType::Grass, CornerType::Grass, CornerType::Grass, CornerType::Town},
                .connections = Connection::NorthWest | Connection::SouthEast | Connection::CrossEastXY,
                .pennant = true,
        },
        {
                .edges{EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Town},
                .corners{CornerType::Town, CornerType::Grass, CornerType::Grass, CornerType::Town},
                .connections = Connection::NorthEast | Connection::NorthWest | Connection::WestEast | Connection::SouthYY,
                .pennant = true,
        },
        {
                .edges{EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Town},
                .corners{CornerType::Town, CornerType::Grass, CornerType::Grass, CornerType::Town},
                .connections = Connection::NorthEast | Connection::NorthWest | Connection::WestEast | Connection::SouthYY,
        },
        {
                .edges{EdgeType::Town, EdgeType::Town, EdgeType::Path, EdgeType::Town},
                .corners{CornerType::Town, CornerType::Grass, CornerType::Grass, CornerType::Town},
                .connections = Connection::NorthEast | Connection::NorthWest | Connection::WestEast,
                .pennant = true,
        },
        {
                .edges{EdgeType::Town, EdgeType::Town, EdgeType::Path, EdgeType::Town},
                .corners{CornerType::Town, CornerType::Grass, CornerType::Grass, CornerType::Town},
                .connections = Connection::NorthEast | Connection::NorthWest | Connection::WestEast,
        },
        {
                .edges{EdgeType::Path, EdgeType::Grass, EdgeType::Path, EdgeType::Grass},
                .corners{CornerType::Grass, CornerType::Grass, CornerType::Grass, CornerType::Grass},
                .connections = Connection::NorthSouth | Connection::West | Connection::East,
        },
        {
                .edges{EdgeType::Grass, EdgeType::Grass, EdgeType::Path, EdgeType::Path},
                .corners{CornerType::Grass, CornerType::Grass, CornerType::Grass, CornerType::Grass},
                .connections = Connection::SouthWest | Connection::North | Connection::East | Connection::CrossWest,
        },
        {
                .edges{EdgeType::Grass, EdgeType::Path, EdgeType::Path, EdgeType::Path},
                .corners{CornerType::Grass, CornerType::Grass, CornerType::Grass, CornerType::Grass},
                .connections = Connection::North,
        },
        {
                .edges{EdgeType::Path, EdgeType::Path, EdgeType::Path, EdgeType::Path},
                .corners{CornerType::Grass, CornerType::Grass, CornerType::Grass, CornerType::Grass},
        },
        {
                .edges{EdgeType::Town, EdgeType::Grass, EdgeType::Town, EdgeType::Grass},
                .corners{CornerType::Grass, CornerType::Grass, CornerType::Grass, CornerType::Grass},
                .connections = Connection::NorthSouth | Connection::WestXX | Connection::EastXX,
                .pennant = true,
        },
};

using TileType = mb::u8;

struct TilePlacement {
   TileType type = 0;
   mb::u8 rotation = 0;

   [[nodiscard]] constexpr Tile tile() const {
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
   NorthEast,
   NorthWest,
   SouthWest,
   SouthEast,
};

constexpr std::array<Direction, 9> g_directions{
        Direction::North,
        Direction::East,
        Direction::South,
        Direction::West,
        Direction::Middle,
        Direction::NorthEast,
        Direction::NorthWest,
        Direction::SouthWest,
        Direction::SouthEast,
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
   case Direction::NorthEast:
      return std::make_tuple(static_cast<double>(tp.x) + 0.1, static_cast<double>(tp.y) + 0.1);
   case Direction::NorthWest:
      return std::make_tuple(static_cast<double>(tp.x) + 0.9, static_cast<double>(tp.y) + 0.1);
   case Direction::SouthWest:
      return std::make_tuple(static_cast<double>(tp.x) + 0.1, static_cast<double>(tp.y) + 0.9);
   case Direction::SouthEast:
      return std::make_tuple(static_cast<double>(tp.x) + 0.9, static_cast<double>(tp.y) + 0.9);
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
   if (c & Connection::North)
      return std::make_tuple(c - Connection::North, Direction::NorthWest, Direction::NorthEast);
   if (c & Connection::West)
      return std::make_tuple(c - Connection::West, Direction::NorthWest, Direction::SouthWest);
   if (c & Connection::South)
      return std::make_tuple(c - Connection::South, Direction::SouthWest, Direction::SouthEast);
   if (c & Connection::East)
      return std::make_tuple(c - Connection::East, Direction::NorthEast, Direction::SouthEast);
   if (c & Connection::CrossWest)
      return std::make_tuple(c - Connection::CrossWest, Direction::NorthWest, Direction::SouthEast);
   if (c & Connection::CrossEast)
      return std::make_tuple(c - Connection::CrossEast, Direction::NorthEast, Direction::SouthWest);

   if (c & Connection::NorthXX)
      return std::make_tuple(c - Connection::NorthXX, Direction::NorthWest, Direction::NorthEast);
   if (c & Connection::NorthXY)
      return std::make_tuple(c - Connection::NorthXY, Direction::NorthWest, Direction::NorthEast);
   if (c & Connection::NorthYX)
      return std::make_tuple(c - Connection::NorthYX, Direction::NorthWest, Direction::NorthEast);
   if (c & Connection::NorthYY)
      return std::make_tuple(c - Connection::NorthYY, Direction::NorthWest, Direction::NorthEast);
   if (c & Connection::NorthXYiX)
      return std::make_tuple(c - Connection::North, Direction::NorthWest, Direction::NorthEast);
   if (c & Connection::NorthXYiY)
      return std::make_tuple(c - Connection::North, Direction::NorthWest, Direction::NorthEast);
   if (c & Connection::NorthXiXY)
      return std::make_tuple(c - Connection::North, Direction::NorthWest, Direction::NorthEast);
   if (c & Connection::NorthYiXY)
      return std::make_tuple(c - Connection::North, Direction::NorthWest, Direction::NorthEast);

   if (c & Connection::EastXX)
      return std::make_tuple(c - Connection::East, Direction::NorthEast, Direction::SouthEast);
   if (c & Connection::EastXY)
      return std::make_tuple(c - Connection::East, Direction::NorthEast, Direction::SouthEast);
   if (c & Connection::EastYX)
      return std::make_tuple(c - Connection::East, Direction::NorthEast, Direction::SouthEast);
   if (c & Connection::EastYY)
      return std::make_tuple(c - Connection::East, Direction::NorthEast, Direction::SouthEast);
   if (c & Connection::EastXYiX)
      return std::make_tuple(c - Connection::East, Direction::NorthEast, Direction::SouthEast);
   if (c & Connection::EastXYiY)
      return std::make_tuple(c - Connection::East, Direction::NorthEast, Direction::SouthEast);
   if (c & Connection::EastXiXY)
      return std::make_tuple(c - Connection::East, Direction::NorthEast, Direction::SouthEast);
   if (c & Connection::EastYiXY)
      return std::make_tuple(c - Connection::East, Direction::NorthEast, Direction::SouthEast);

   if (c & Connection::SouthXX)
      return std::make_tuple(c - Connection::South, Direction::SouthWest, Direction::SouthEast);
   if (c & Connection::SouthXY)
      return std::make_tuple(c - Connection::South, Direction::SouthWest, Direction::SouthEast);
   if (c & Connection::SouthYX)
      return std::make_tuple(c - Connection::South, Direction::SouthWest, Direction::SouthEast);
   if (c & Connection::SouthYY)
      return std::make_tuple(c - Connection::South, Direction::SouthWest, Direction::SouthEast);
   if (c & Connection::SouthXYiX)
      return std::make_tuple(c - Connection::South, Direction::SouthWest, Direction::SouthEast);
   if (c & Connection::SouthXYiY)
      return std::make_tuple(c - Connection::South, Direction::SouthWest, Direction::SouthEast);
   if (c & Connection::SouthXiXY)
      return std::make_tuple(c - Connection::South, Direction::SouthWest, Direction::SouthEast);
   if (c & Connection::SouthYiXY)
      return std::make_tuple(c - Connection::South, Direction::SouthWest, Direction::SouthEast);

   if (c & Connection::WestXX)
      return std::make_tuple(c - Connection::West, Direction::NorthWest, Direction::SouthWest);
   if (c & Connection::WestXY)
      return std::make_tuple(c - Connection::West, Direction::NorthWest, Direction::SouthWest);
   if (c & Connection::WestYX)
      return std::make_tuple(c - Connection::West, Direction::NorthWest, Direction::SouthWest);
   if (c & Connection::WestYY)
      return std::make_tuple(c - Connection::West, Direction::NorthWest, Direction::SouthWest);
   if (c & Connection::WestXYiX)
      return std::make_tuple(c - Connection::West, Direction::NorthWest, Direction::SouthWest);
   if (c & Connection::WestXYiY)
      return std::make_tuple(c - Connection::West, Direction::NorthWest, Direction::SouthWest);
   if (c & Connection::WestXiXY)
      return std::make_tuple(c - Connection::West, Direction::NorthWest, Direction::SouthWest);
   if (c & Connection::WestYiXY)
      return std::make_tuple(c - Connection::West, Direction::NorthWest, Direction::SouthWest);

   if (c & Connection::CrossWestXX)
      return std::make_tuple(c - Connection::CrossWest, Direction::NorthWest, Direction::SouthEast);
   if (c & Connection::CrossWestXY)
      return std::make_tuple(c - Connection::CrossWest, Direction::NorthWest, Direction::SouthEast);
   if (c & Connection::CrossWestYX)
      return std::make_tuple(c - Connection::CrossWest, Direction::NorthWest, Direction::SouthEast);
   if (c & Connection::CrossWestYY)
      return std::make_tuple(c - Connection::CrossWest, Direction::NorthWest, Direction::SouthEast);
   if (c & Connection::CrossWestXYiX)
      return std::make_tuple(c - Connection::CrossWest, Direction::NorthWest, Direction::SouthEast);
   if (c & Connection::CrossWestXYiY)
      return std::make_tuple(c - Connection::CrossWest, Direction::NorthWest, Direction::SouthEast);
   if (c & Connection::CrossWestXiXY)
      return std::make_tuple(c - Connection::CrossWest, Direction::NorthWest, Direction::SouthEast);
   if (c & Connection::CrossWestYiXY)
      return std::make_tuple(c - Connection::CrossWest, Direction::NorthWest, Direction::SouthEast);

   if (c & Connection::CrossEastXX)
      return std::make_tuple(c - Connection::CrossEast, Direction::NorthEast, Direction::SouthWest);
   if (c & Connection::CrossEastXY)
      return std::make_tuple(c - Connection::CrossEast, Direction::NorthEast, Direction::SouthWest);
   if (c & Connection::CrossEastYX)
      return std::make_tuple(c - Connection::CrossEast, Direction::NorthEast, Direction::SouthWest);
   if (c & Connection::CrossEastYY)
      return std::make_tuple(c - Connection::CrossEast, Direction::NorthEast, Direction::SouthWest);
   if (c & Connection::CrossEastXYiX)
      return std::make_tuple(c - Connection::CrossEast, Direction::NorthEast, Direction::SouthWest);
   if (c & Connection::CrossEastXYiY)
      return std::make_tuple(c - Connection::CrossEast, Direction::NorthEast, Direction::SouthWest);
   if (c & Connection::CrossEastXiXY)
      return std::make_tuple(c - Connection::CrossEast, Direction::NorthEast, Direction::SouthWest);
   if (c & Connection::CrossEastYiXY)
      return std::make_tuple(c - Connection::CrossEast, Direction::NorthEast, Direction::SouthWest);
   
   return std::make_tuple(Connection::None, Direction::Middle, Direction::Middle);
}

using Edge = int;

constexpr auto g_west_east_edges = g_board_width + (g_board_height + 1);
constexpr auto g_middle_edges = g_west_east_edges + (g_board_width + 1) + g_board_height;
constexpr auto g_corner_edges = g_middle_edges + g_board_width * g_board_height;

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
   case Direction::NorthEast:
      return static_cast<Edge>(g_corner_edges + (x + 1) + y * g_board_width);
   case Direction::NorthWest:
      return static_cast<Edge>(g_corner_edges + x + y * g_board_width);
   case Direction::SouthWest:
      return static_cast<Edge>(g_corner_edges + x + (y + 1) * g_board_width);
   case Direction::SouthEast:
      return static_cast<Edge>(g_corner_edges + (x + 1) + (y + 1) * g_board_width);
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
