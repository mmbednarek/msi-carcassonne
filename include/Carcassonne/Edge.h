#ifndef MSI_CARCASSONNE_EDGE_H
#define MSI_CARCASSONNE_EDGE_H

#include <limits>
namespace carcassonne {

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
   return static_cast<Edge>(std::numeric_limits<Edge>::infinity());
}

}

#endif//MSI_CARCASSONNE_EDGE_H
