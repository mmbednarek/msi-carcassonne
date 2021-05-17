#ifndef MSI_CARCASSONNE_CORE_H
#define MSI_CARCASSONNE_CORE_H
#include <Carcassonne/Tiles.h>
#include <Carcassonne/Direction.h>
#include <Carcassonne/Player.h>
#include <memory>
#include <mb/view.h>

namespace carcassonne {

constexpr mb::size g_board_width = 141;
constexpr mb::size g_board_height = 141;

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

constexpr TilePosition neighbour_of(int x, int y, Direction d) {
   switch (d) {
   case Direction::North:
      return TilePosition{x, y - 1};
   case Direction::East:
      return TilePosition{x + 1, y};
   case Direction::South:
      return TilePosition{x, y + 1};
   case Direction::West:
      return TilePosition{x - 1, y};
   default:
      return TilePosition{x, y};
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

[[nodiscard]] constexpr std::tuple<Contact, Direction, Direction> read_contacts(Contact c) {
   if (c & Contact::NorthNWX)
      return std::make_tuple(c - Contact::NorthNWX, Direction::North, Direction::WestNorth);
   if (c & Contact::NorthNEX)
      return std::make_tuple(c - Contact::NorthNEX, Direction::North, Direction::EastNorth);
   if (c & Contact::NorthSWX)
      return std::make_tuple(c - Contact::NorthSWX, Direction::North, Direction::WestSouth);
   if (c & Contact::NorthSWY)
      return std::make_tuple(c - Contact::NorthSWY, Direction::North, Direction::SouthWest);
   if (c & Contact::NorthSEX)
      return std::make_tuple(c - Contact::NorthSEX, Direction::North, Direction::EastSouth);
   if (c & Contact::NorthSEY)
      return std::make_tuple(c - Contact::NorthSEY, Direction::North, Direction::SouthEast);

   if (c & Contact::EastNEY)
      return std::make_tuple(c - Contact::EastNEY, Direction::East, Direction::NorthEast);
   if (c & Contact::EastSEY)
      return std::make_tuple(c - Contact::EastSEY, Direction::East, Direction::SouthEast);
   if (c & Contact::EastNWY)
      return std::make_tuple(c - Contact::EastNWY, Direction::East, Direction::NorthWest);
   if (c & Contact::EastNWX)
      return std::make_tuple(c - Contact::EastNWX, Direction::East, Direction::WestNorth);
   if (c & Contact::EastSWY)
      return std::make_tuple(c - Contact::EastSWY, Direction::East, Direction::SouthWest);
   if (c & Contact::EastSWX)
      return std::make_tuple(c - Contact::EastSWX, Direction::East, Direction::WestSouth);

   if (c & Contact::SouthSEX)
      return std::make_tuple(c - Contact::SouthSEX, Direction::South, Direction::EastSouth);
   if (c & Contact::SouthSWX)
      return std::make_tuple(c - Contact::SouthSWX, Direction::South, Direction::WestSouth);
   if (c & Contact::SouthNEX)
      return std::make_tuple(c - Contact::SouthNEX, Direction::South, Direction::EastNorth);
   if (c & Contact::SouthNEY)
      return std::make_tuple(c - Contact::SouthNEY, Direction::South, Direction::NorthEast);
   if (c & Contact::SouthNWX)
      return std::make_tuple(c - Contact::SouthNWX, Direction::South, Direction::WestNorth);
   if (c & Contact::SouthNWY)
      return std::make_tuple(c - Contact::SouthNWY, Direction::South, Direction::NorthWest);

   if (c & Contact::WestSWY)
      return std::make_tuple(c - Contact::WestSWY, Direction::South, Direction::SouthWest);
   if (c & Contact::WestNWY)
      return std::make_tuple(c - Contact::WestNWY, Direction::South, Direction::NorthWest);
   if (c & Contact::WestSEY)
      return std::make_tuple(c - Contact::WestSEY, Direction::South, Direction::SouthEast);
   if (c & Contact::WestSEX)
      return std::make_tuple(c - Contact::WestSEX, Direction::South, Direction::EastSouth);
   if (c & Contact::WestNEY)
      return std::make_tuple(c - Contact::WestNEY, Direction::South, Direction::NorthEast);
   if (c & Contact::WestNEX)
      return std::make_tuple(c - Contact::WestNEX, Direction::South, Direction::EastNorth);

   return std::make_tuple(Contact::None, Direction::Middle, Direction::Middle);
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

using Edge = mb::u32;

constexpr auto g_edges_horizontal = 0;
constexpr auto g_edges_vertical = g_edges_horizontal + g_board_width * (g_board_height + 1);
constexpr auto g_edges_middle = g_edges_vertical + (g_board_width + 1) * g_board_height;
constexpr auto g_edges_field_horizontal_west = g_edges_middle + g_board_width * g_board_height;
constexpr auto g_edges_field_horizontal_east = g_edges_field_horizontal_west + g_board_width * (g_board_height + 1);
constexpr auto g_edges_field_vertical_north = g_edges_field_horizontal_east + g_board_width * (g_board_height + 1);
constexpr auto g_edges_field_vertical_south = g_edges_field_vertical_north + (g_board_width + 1) * g_board_height;
constexpr auto g_edges_max = g_edges_field_vertical_south + (g_board_width + 1) * g_board_height;
constexpr auto g_fields_max = 100;

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
   return static_cast<Edge>(std::numeric_limits<Edge>::infinity());
}

struct Figure {
   Player player;
   double x, y;
   Edge edge;
};

constexpr Player next_player(Player player, mb::u8 player_count) {
   return static_cast<Player>((static_cast<int>(player) + 1) % player_count);
}

enum class MovePhase {
   PlaceTile,
   PlaceFigure,
   Done
};

class ScoreBoard;

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
   [[nodiscard]] virtual const ScoreBoard &scores() const noexcept = 0;
};

}// namespace carcassonne

#endif//MSI_CARCASSONNE_CORE_H