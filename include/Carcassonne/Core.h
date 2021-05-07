#ifndef MSI_CARCASSONNE_CORE_H
#define MSI_CARCASSONNE_CORE_H
#include <algorithm>
#include <array>
#include <mb/int.h>
#include <mb/view.h>
#include <memory>
#include <Uint128_t/uint128_t.h>

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
   NorthEast = 1 << 0,
   NorthSouth = 1 << 1,
   NorthWest = 1 << 2,
   SouthEast = 1 << 3,
   SouthWest = 1 << 4,
   WestEast = 1 << 5,
   NorthRightSouthRight = 1 << 6, // 0
   NorthLeftSouthLeft = 1 << 7, // 0
   NorthLeftWest = 1 << 8, // 0
   SouthLeftWest = 1 << 9, // 0
   SouthLeftEastUp = 1 << 10, // 8
   SouthRightEastDown = 1 << 11, // 8
   WestEastUp = 1 << 12, // 8
   SouthRightWestUp = 1 << 13, // 9
   SouthLeftWestDown = 1 << 14, // 9
   WestUpEast = 1 << 15, // 9
   WestUpEastUp = 1 << 16, // 10
   NorthRightEast = 1 << 17, // 19
   SouthRightEast = 1 << 18, // 19
   NorthWestUp = 1 << 19, // 20
   NorthSouthRight = 1 << 20, // 20
   NorthEastUp = 1 << 21, // 21
   NorthLeftWestUp = 1 << 22, // 22
   NorthRightEastUp = 1 << 23, // 22
   WestDownEastDown = 1 << 24,
   SouthWestDown = 1 << 25,
   SouthEastDown = 1 << 26,
   NorthRightWestDown = 1 << 27,
   NorthLeftEastDown = 1 << 28,
   WestDownEast = 1 << 29,
   NorthLeftSouth = 1 << 30,
   // NorthRightSouth = 1 << 31, // enumeration value is outside the range of its underlying type ("unsigned long")C/C++(1749)
   All = 0x3f,
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

   if (c & Connection::NorthRightSouthRight)
      result |= Connection::WestDownEastDown;
   if (c & Connection::WestDownEastDown)
      result |= Connection::NorthLeftSouthLeft;
   if (c & Connection::NorthLeftSouthLeft)
      result |= Connection::WestUpEastUp;
   if (c & Connection::WestUpEastUp)
      result |= Connection::NorthRightSouthRight;

   if (c & Connection::NorthLeftWest)
      result |= Connection::NorthEastUp;
   if (c & Connection::NorthEastUp)
      result |= Connection::SouthRightEast;
   if (c & Connection::SouthRightEast)
      result |= Connection::SouthWestDown;
   if (c & Connection::SouthWestDown)
      result |= Connection::NorthLeftWest;


   if (c & Connection::SouthLeftWest)
      result |= Connection::NorthWestUp;
   if (c & Connection::NorthWestUp)
      result |= Connection::NorthRightEast;
   if (c & Connection::NorthRightEast)
      result |= Connection::SouthEastDown;
   if (c & Connection::SouthEastDown)
      result |= Connection::SouthLeftWest;

   if (c & Connection::SouthLeftEastUp)
      result |= Connection::SouthRightWestUp;
   if (c & Connection::SouthRightWestUp)
      result |= Connection::NorthRightWestDown;
   if (c & Connection::NorthRightWestDown)
      result |= Connection::NorthLeftEastDown;
   if (c & Connection::NorthLeftEastDown)
      result |= Connection::SouthLeftEastUp;

   if (c & Connection::SouthRightEastDown)
      result |= Connection::SouthLeftWestDown;
   if (c & Connection::SouthLeftWestDown)
      result |= Connection::NorthLeftWestUp;
   if (c & Connection::NorthLeftWestUp)
      result |= Connection::NorthRightEastUp;
   if (c & Connection::NorthRightEastUp)
      result |= Connection::SouthRightEastDown;

   if (c & Connection::WestEastUp)
      result |= Connection::NorthSouthRight;
   if (c & Connection::NorthSouthRight)
      result |= Connection::WestDownEast;
   if (c & Connection::WestDownEast)
      result |= Connection::NorthLeftSouth;
   if (c & Connection::NorthLeftSouth)
      result |= Connection::WestEastUp;

   // if (c & Connection::WestUpEast)
   //    result |= Connection::NorthRightSouth;
   // if (c & Connection::NorthRightSouth)
   //    result |= Connection::WestDownEast;
   if (c & Connection::WestDownEast)
      result |= Connection::NorthLeftSouth;
   if (c & Connection::NorthLeftSouth)
      result |= Connection::WestUpEast;

   if (c & Connection::WestUpEastUp)
      result |= Connection::NorthRightSouthRight;
   if (c & Connection::NorthRightSouthRight)
      result |= Connection::WestDownEastDown;
   if (c & Connection::WestDownEastDown)
      result |= Connection::NorthLeftSouthLeft;
   if (c & Connection::NorthLeftSouthLeft)
      result |= Connection::WestUpEastUp;
   
   return result;
}

struct Tile {
   std::array<EdgeType, 4> edges{};
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
      std::rotate(result.edges.begin(), result.edges.end() - count, result.edges.end());
      return result;
   }
};

constexpr std::array<Tile, 24> g_tiles{
        Tile{
                .edges{EdgeType::Path, EdgeType::Town, EdgeType::Path, EdgeType::Grass},
                .connections = Connection::NorthSouth | Connection::NorthLeftWest | Connection::SouthLeftWest | Connection::NorthLeftSouthLeft | Connection::NorthRightSouthRight,
        },
        {
                .edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass},
                .connections = Connection::WestEast | Connection::SouthWest | Connection::SouthEast,
        },
        {
                .edges{EdgeType::Town, EdgeType::Grass, EdgeType::Town, EdgeType::Grass},
                .connections = Connection::NorthSouth,
        },
        {
                .edges{EdgeType::Grass, EdgeType::Town, EdgeType::Grass, EdgeType::Town},
                .connections = Connection::NorthSouth
        },
        {
                .edges{EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass},
                .connections = Connection::NorthEast | Connection::NorthSouth | Connection::NorthWest | Connection::SouthEast | Connection::SouthWest | Connection::WestEast,
                .monastery = true,
        },
        {
                .edges{EdgeType::Grass, EdgeType::Grass, EdgeType::Path, EdgeType::Grass},
                .connections = Connection::NorthEast | Connection::NorthWest | Connection::WestEast,
                .monastery = true,
        },
        {
                .edges{EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Grass},
                .connections = Connection::NorthWest,
        },
        {
                .edges{EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Town},
                .connections = Connection::All,
                .pennant = true,
        },
        {
                .edges{EdgeType::Town, EdgeType::Path, EdgeType::Path, EdgeType::Grass},
                .connections = Connection::SouthEast | Connection::WestEastUp | Connection::SouthLeftWest | Connection::SouthLeftEastUp | Connection::SouthRightEastDown,
        },
        {
                .edges{EdgeType::Town, EdgeType::Grass, EdgeType::Path, EdgeType::Path},
                .connections = Connection::SouthWest | Connection::WestUpEast | Connection::SouthRightEast | Connection::SouthRightWestUp | Connection::SouthLeftWestDown,
        },
        {
                .edges{EdgeType::Town, EdgeType::Path, EdgeType::Path, EdgeType::Path},
                .connections = Connection::WestUpEastUp | Connection::SouthLeftWestDown | Connection::SouthRightEastDown,
        },
        {
                .edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Town},
                .connections = Connection::NorthWest | Connection::SouthEast,
                .pennant = true,
        },
        {
                .edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Town},
                .connections = Connection::NorthWest | Connection::SouthEast,
        },
        {
                .edges{EdgeType::Town, EdgeType::Path, EdgeType::Path, EdgeType::Town},
                .connections = Connection::NorthWest | Connection::SouthEast | Connection::SouthLeftEastUp | Connection::SouthRightEastDown,
        },
        {
                .edges{EdgeType::Town, EdgeType::Path, EdgeType::Path, EdgeType::Town},
                .connections = Connection::NorthWest | Connection::SouthEast | Connection::SouthLeftEastUp | Connection::SouthRightEastDown,
                .pennant = true,
        },
        {
                .edges{EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Town},
                .connections = Connection::NorthEast | Connection::NorthWest | Connection::WestEast,
                .pennant = true,
        },
        {
                .edges{EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Town},
                .connections = Connection::NorthEast | Connection::NorthWest | Connection::WestEast,
        },
        {
                .edges{EdgeType::Town, EdgeType::Town, EdgeType::Path, EdgeType::Town},
                .connections = Connection::NorthEast | Connection::NorthWest | Connection::WestEast,
                .pennant = true,
        },
        {
                .edges{EdgeType::Town, EdgeType::Town, EdgeType::Path, EdgeType::Town},
                .connections = Connection::NorthEast | Connection::NorthWest | Connection::WestEast,
        },
        {
                .edges{EdgeType::Path, EdgeType::Grass, EdgeType::Path, EdgeType::Grass},
                .connections = Connection::NorthSouth | Connection::NorthLeftSouthLeft | Connection::NorthLeftWest | Connection::SouthLeftWest | Connection::NorthRightSouthRight | Connection::NorthRightEast | Connection::SouthRightEast,
        },
        {
                .edges{EdgeType::Grass, EdgeType::Grass, EdgeType::Path, EdgeType::Path},
                .connections = Connection::SouthWest | Connection::NorthEast | Connection::NorthSouthRight | Connection::NorthWestUp | Connection::SouthRightWestUp | Connection::SouthRightEast | Connection::WestUpEast,
        },
        {
                .edges{EdgeType::Grass, EdgeType::Path, EdgeType::Path, EdgeType::Path},
                .connections = Connection::WestUpEastUp | Connection::NorthWestUp | Connection::NorthEastUp | Connection::SouthLeftWestDown | Connection::SouthRightEastDown,
        },
        {
                .edges{EdgeType::Path, EdgeType::Path, EdgeType::Path, EdgeType::Path},
                .connections = Connection::SouthLeftWestDown | Connection::SouthRightEastDown | Connection::NorthLeftWestUp | Connection::NorthRightEastUp,
        },
        {
                .edges{EdgeType::Town, EdgeType::Grass, EdgeType::Town, EdgeType::Grass},
                .connections = Connection::NorthSouth,
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
   NorthRight,
   NorthLeft,
   SouthRight,
   SouthLeft,
   WestUp,
   WestDown,
   EastUp,
   EastDown,
};

constexpr std::array<Direction, 13> g_directions{
        Direction::North,
        Direction::East,
        Direction::South,
        Direction::West,
        Direction::Middle,
        Direction::NorthLeft,
        Direction::NorthRight,
        Direction::SouthLeft,
        Direction::SouthRight,
        Direction::WestUp,
        Direction::WestDown,
        Direction::EastUp,
        Direction::EastDown,
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
   case Direction::NorthLeft:
      return std::make_tuple(static_cast<double>(tp.x) +0.25, static_cast<double>(tp.y) + 0.1);
   case Direction::NorthRight:
      return std::make_tuple(static_cast<double>(tp.x) +0.75, static_cast<double>(tp.y) + 0.1);
   case Direction::SouthLeft:
      return std::make_tuple(static_cast<double>(tp.x) +0.25, static_cast<double>(tp.y) + 0.9);
   case Direction::SouthRight:
      return std::make_tuple(static_cast<double>(tp.x) +0.75, static_cast<double>(tp.y) + 0.9);
   case Direction::WestUp:
      return std::make_tuple(static_cast<double>(tp.x) + 0.1, static_cast<double>(tp.y) +0.25);
   case Direction::WestDown:
      return std::make_tuple(static_cast<double>(tp.x) + 0.1, static_cast<double>(tp.y) +0.75);
   case Direction::EastUp:
      return std::make_tuple(static_cast<double>(tp.x) + 0.9, static_cast<double>(tp.y) +0.25);
   case Direction::EastDown:
      return std::make_tuple(static_cast<double>(tp.x) + 0.9, static_cast<double>(tp.y) +0.75);
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
   if (c & Connection::NorthRightSouthRight)
      return std::make_tuple(c - Connection::NorthRightSouthRight, Direction::NorthRight, Direction::SouthRight);
   if (c & Connection::NorthLeftSouthLeft)
      return std::make_tuple(c - Connection::NorthLeftSouthLeft, Direction::NorthLeft, Direction::SouthLeft);
   if (c & Connection::NorthLeftWest)
      return std::make_tuple(c - Connection::NorthLeftWest, Direction::NorthLeft, Direction::West);
   if (c & Connection::SouthLeftWest)
      return std::make_tuple(c - Connection::SouthLeftWest, Direction::SouthLeft, Direction::West);
   if (c & Connection::SouthLeftEastUp)
      return std::make_tuple(c - Connection::SouthLeftEastUp, Direction::SouthLeft, Direction::EastUp);
   if (c & Connection::SouthRightEastDown)
      return std::make_tuple(c - Connection::SouthRightEastDown, Direction::SouthRight, Direction::EastDown);
   if (c & Connection::WestEastUp)
      return std::make_tuple(c - Connection::WestEastUp, Direction::West, Direction::EastUp);
   if (c & Connection::SouthRightWestUp)
      return std::make_tuple(c - Connection::SouthRightWestUp, Direction::SouthRight, Direction::WestUp);
   if (c & Connection::SouthLeftWestDown)
      return std::make_tuple(c - Connection::SouthLeftWestDown, Direction::SouthLeft, Direction::WestDown);
   if (c & Connection::WestUpEast)
      return std::make_tuple(c - Connection::WestUpEast, Direction::WestUp, Direction::East);
   if (c & Connection::WestUpEastUp)
      return std::make_tuple(c - Connection::WestUpEastUp, Direction::WestUp, Direction::EastUp);
   if (c & Connection::NorthRightEast)
      return std::make_tuple(c - Connection::NorthRightEast, Direction::NorthRight, Direction::East);
   if (c & Connection::SouthRightEast)
      return std::make_tuple(c - Connection::SouthRightEast, Direction::SouthRight, Direction::East);
   if (c & Connection::NorthWestUp)
      return std::make_tuple(c - Connection::NorthWestUp, Direction::North, Direction::WestUp);
   if (c & Connection::NorthSouthRight)
      return std::make_tuple(c - Connection::NorthSouthRight, Direction::North, Direction::SouthRight);
   if (c & Connection::NorthEastUp)
      return std::make_tuple(c - Connection::NorthEastUp, Direction::North, Direction::EastUp);
   if (c & Connection::NorthLeftWestUp)
      return std::make_tuple(c - Connection::NorthLeftWestUp, Direction::NorthLeft, Direction::WestUp);
   if (c & Connection::NorthRightEastUp)
      return std::make_tuple(c - Connection::NorthRightEastUp, Direction::NorthRight, Direction::EastUp);
   if (c & Connection::WestDownEastDown)
      return std::make_tuple(c - Connection::WestDownEastDown, Direction::WestDown, Direction::EastDown);
   if (c & Connection::SouthWestDown)
      return std::make_tuple(c - Connection::SouthWestDown, Direction::South, Direction::WestDown);
   if (c & Connection::SouthEastDown)
      return std::make_tuple(c - Connection::SouthEastDown, Direction::South, Direction::EastDown);
   if (c & Connection::NorthRightWestDown)
      return std::make_tuple(c - Connection::NorthRightWestDown, Direction::NorthRight, Direction::WestDown);
   if (c & Connection::NorthLeftEastDown)
      return std::make_tuple(c - Connection::NorthLeftEastDown, Direction::NorthLeft, Direction::EastDown);
   if (c & Connection::WestDownEast)
      return std::make_tuple(c - Connection::WestDownEast, Direction::WestDown, Direction::East);
   if (c & Connection::NorthLeftSouth)
      return std::make_tuple(c - Connection::NorthLeftSouth, Direction::NorthLeft, Direction::South);
   // if (c & Connection::NorthRightSouth)
   //    return std::make_tuple(c - Connection::NorthRightSouth, Direction::NorthRight, Direction::South);
   
   return std::make_tuple(Connection::None, Direction::Middle, Direction::Middle);
}

using Edge = int;

constexpr auto g_west_east_edges = g_board_width + (g_board_height + 1);
constexpr auto g_middle_edges = g_west_east_edges + (g_board_width + 1) + g_board_height;

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
   // case Direction::NorthLeft:
   //    return static_cast<Edge>(x + y * g_board_width);
   // case Direction::NorthRight:
   //    return static_cast<Edge>(x + y * g_board_width);
   // case Direction::SouthLeft:
   //    return static_cast<Edge>(x + y * g_board_width);
   // case Direction::SouthRight:
   //    return static_cast<Edge>(x + y * g_board_width);
   // case Direction::WestUp:
   //    return static_cast<Edge>(x + y * g_board_width);
   // case Direction::WestDown:
   //    return static_cast<Edge>(x + y * g_board_width);
   // case Direction::EastUp:
   //    return static_cast<Edge>(x + y * g_board_width);
   // case Direction::EastDown:
   //    return static_cast<Edge>(x + y * g_board_width);

   // Tutaj skonczylem, rozszerzenia o pola i farmerow do kontynuacji w nastepnych liniach kodu
   }
   return 0;
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
