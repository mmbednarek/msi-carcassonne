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
   NorthEastCross = 1 << 12,
   SouthEastCross = 1 << 13,
   SouthWestCross = 1 << 14,
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

   if (c & Connection::NorthEastCross)
      result |= Connection::SouthEastCross;
   if (c & Connection::SouthEastCross)
      result |= Connection::SouthWestCross;
   if (c & Connection::SouthWestCross)
      result |= Connection::NorthWestCross;
   if (c & Connection::NorthWestCross)
      result |= Connection::NorthEastCross;

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
      std::rotate(result.field_edges.begin(), result.field_edges.end() - 2 * count, result.field_edges.end());

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
                .connections = Connection::NorthSouth | Connection::EastX | Connection::West,
        },
        {
                .edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass},
                .field_edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Town},
                .connections = Connection::EastY | Connection::South | Connection::WestY,
        },
        {
                .edges{EdgeType::Town, EdgeType::Grass, EdgeType::Town, EdgeType::Grass},
                .field_edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Town},
                .connections = Connection::NorthSouth | Connection::EastY | Connection::WestY,
        },
        {
                .edges{EdgeType::Grass, EdgeType::Town, EdgeType::Grass, EdgeType::Town},
                .field_edges{EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Grass},
                .connections = Connection::NorthX | Connection::SouthX | Connection::WestX | Connection::EastX,
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
                .connections = Connection::AllFieldEdges,
                .monastery = true,
        },
        {
                .edges{EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Grass},
                .field_edges{EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass},
                .connections = Connection::NorthWestCorner | Connection::NorthX | Connection::WestY,
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
                .connections = Connection::SouthEast | Connection::WestY | Connection::SouthWestCorner | Connection::NorthY | Connection::SouthEastCorner,
        },
        {
                .edges{EdgeType::Town, EdgeType::Grass, EdgeType::Path, EdgeType::Path},
                .field_edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Town},
                .connections = Connection::SouthWest | Connection::EastY | Connection::SouthWestCorner | Connection::NorthY | Connection::SouthEastCorner,
        },
        {
                .edges{EdgeType::Town, EdgeType::Path, EdgeType::Path, EdgeType::Path},
                .field_edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Town},
                .connections = Connection::NorthY | Connection::SouthEastCorner | Connection::SouthWestCorner,
        },
        {
                .edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Town},
                .field_edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Town},
                .connections = Connection::NorthWest | Connection::SouthX | Connection::EastY | Connection::SouthEastCorner,
                .pennant = true,
        },
        {
                .edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Town},
                .field_edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Town},
                .connections = Connection::NorthWest | Connection::SouthX | Connection::EastY | Connection::SouthEastCorner,
        },
        {
                .edges{EdgeType::Town, EdgeType::Path, EdgeType::Path, EdgeType::Town},
                .field_edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Town},
                .connections = Connection::NorthWest | Connection::SouthEast | Connection::SouthEastCorner | Connection::SouthEastCross,
        },
        {
                .edges{EdgeType::Town, EdgeType::Path, EdgeType::Path, EdgeType::Town},
                .field_edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Town},
                .connections = Connection::NorthWest | Connection::SouthEast | Connection::SouthEastCorner | Connection::SouthEastCross,
                .pennant = true,
        },
        {
                .edges{EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Town},
                .field_edges{EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Town},
                .connections = Connection::NorthEast | Connection::NorthWest | Connection::WestEast | Connection::SouthX,
                .pennant = true,
        },
        {
                .edges{EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Town},
                .field_edges{EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Town},
                .connections = Connection::NorthEast | Connection::NorthWest | Connection::WestEast | Connection::SouthX,
        },
        {
                .edges{EdgeType::Town, EdgeType::Town, EdgeType::Path, EdgeType::Town},
                .field_edges{EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Town},
                .connections = Connection::NorthEast | Connection::NorthWest | Connection::WestEast,
                .pennant = true,
        },
        {
                .edges{EdgeType::Town, EdgeType::Town, EdgeType::Path, EdgeType::Town},
                .field_edges{EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Town},
                .connections = Connection::NorthEast | Connection::NorthWest | Connection::WestEast,
        },
        {
                .edges{EdgeType::Path, EdgeType::Grass, EdgeType::Path, EdgeType::Grass},
                .field_edges{EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass},
                .connections = Connection::NorthSouth | Connection::West | Connection::East,
        },
        {
                .edges{EdgeType::Grass, EdgeType::Grass, EdgeType::Path, EdgeType::Path},
                .field_edges{EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass},
                .connections = Connection::SouthWest | Connection::North | Connection::East | Connection::SouthWestCorner,
        },
        {
                .edges{EdgeType::Grass, EdgeType::Path, EdgeType::Path, EdgeType::Path},
                .field_edges{EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass},
                .connections = Connection::North | Connection::SouthEastCorner | Connection::SouthWestCorner,
        },
        {
                .edges{EdgeType::Path, EdgeType::Path, EdgeType::Path, EdgeType::Path},
                .field_edges{EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass},
                .connections = Connection::NorthEastCorner | Connection::SouthEastCorner | Connection::SouthWestCorner | Connection::NorthWestCorner,
        },
        {
                .edges{EdgeType::Town, EdgeType::Grass, EdgeType::Town, EdgeType::Grass},
                .field_edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Town},
                .connections = Connection::NorthSouth | Connection::WestY | Connection::EastY,
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
   NorthEast,
   EastNorth,
   SouthEast,
   EastSouth,
   SouthWest,
   WestSouth,
   NorthWest,
   WestNorth
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

   if (c & Connection::NorthEastCross)
      return std::make_tuple(c - Connection::NorthEastCross, Direction::EastNorth, Direction::SouthWest);
   if (c & Connection::SouthEastCross)
      return std::make_tuple(c - Connection::SouthEastCross, Direction::SouthEast, Direction::WestNorth);
   if (c & Connection::SouthWestCross)
      return std::make_tuple(c - Connection::SouthWestCross, Direction::WestSouth, Direction::NorthEast);
   if (c & Connection::NorthWestCross)
      return std::make_tuple(c - Connection::NorthWestCross, Direction::NorthWest, Direction::EastSouth);

   return std::make_tuple(Connection::None, Direction::Middle, Direction::Middle);
}

using Edge = mb::u32;

constexpr auto g_edges_horizontal = 0;
constexpr auto g_edges_vertical = g_edges_horizontal + g_board_width * (g_board_height + 1);
constexpr auto g_edges_middle = g_edges_vertical + (g_board_width + 1) * g_board_height;
constexpr auto g_edges_field_horizontal_west = g_edges_middle + g_board_width * g_board_height;
constexpr auto g_edges_field_horizontal_east = g_edges_field_horizontal_west + g_board_width * (g_board_height + 1);
constexpr auto g_edges_field_vertical_north = g_edges_field_horizontal_east + g_board_width * (g_board_height + 1);
constexpr auto g_edges_field_vertical_south = g_edges_field_vertical_north + (g_board_width + 1) * g_board_height;
constexpr auto g_edges_max = g_edges_field_vertical_south + (g_board_width + 1) * g_board_height;

constexpr Edge make_edge(int x, int y, Direction d) {
   switch (d) {
   case Direction::North:
      return static_cast<Edge>(g_edges_horizontal + x + y * g_board_width);
   case Direction::South:
      return static_cast<Edge>(g_edges_horizontal + x + (y + 1) * g_board_width);

   case Direction::West:
      return static_cast<Edge>(g_edges_vertical + x + y * g_board_width);
   case Direction::East:
      return static_cast<Edge>(g_edges_vertical + (x + 1) + y * g_board_width);

   case Direction::Middle:
      return static_cast<Edge>(g_edges_middle + x + y * g_board_width);

   case Direction::EastNorth:
      return static_cast<Edge>(g_edges_field_vertical_north + (x + 1) + y * g_board_width);
   case Direction::EastSouth:
      return static_cast<Edge>(g_edges_field_vertical_south + (x + 1) + y * g_board_width);

   case Direction::WestNorth:
      return static_cast<Edge>(g_edges_field_vertical_north + x + y * g_board_width);
   case Direction::WestSouth:
      return static_cast<Edge>(g_edges_field_vertical_south + x + y * g_board_width);

   case Direction::NorthWest:
      return static_cast<Edge>(g_edges_field_horizontal_west + x + y * g_board_width);
   case Direction::NorthEast:
      return static_cast<Edge>(g_edges_field_horizontal_east + x + y * g_board_width);

   case Direction::SouthWest:
      return static_cast<Edge>(g_edges_field_horizontal_west + x + (y + 1) * g_board_width);
   case Direction::SouthEast:
      return static_cast<Edge>(g_edges_field_horizontal_east + x + (y + 1) * (g_board_height));
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