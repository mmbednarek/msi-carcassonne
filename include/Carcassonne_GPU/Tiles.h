#ifndef CARCASSONNE_GPU_TILES_H
#define CARCASSONNE_GPU_TILES_H
#ifdef __CUDACC__
#define DEVHOST __device__ __host__
#else
#define DEVHOST
#endif

#include <Carcassonne_GPU/Connection.h>
#include <Carcassonne_GPU/Contact.h>
#include <algorithm>
#include <limits>

namespace carcassonne {

enum class EdgeType {
   Grass,
   Path,
   Town,
   Monastery
};

struct Tile {
   std::array<EdgeType, 4> edges{};
   std::array<EdgeType, 8> field_edges{};
   mb::u8 amount = std::numeric_limits<mb::u8>::infinity();
   Contact contacts = Contact::None;
   Connection connections = Connection::None;
   bool monastery = false;
   bool pennant = false;
//    Tile() = default;
//    Tile (  const std::array<EdgeType, 4>& edges_, 
//            const std::array<EdgeType, 8>& field_edges_, 
//            mb::u8 amount_ = std::numeric_limits<mb::u8>::infinity(),
//            const Contact& contacts_=Contact::None, 
//            const Connection& connections_=Connection::None, 
//            bool monastery_=false, 
//            bool pennant_=false
//         )
//     : edges(edges_)
//     , field_edges(field_edges_)
//     , amount(amount_)
//     , contacts(contacts_)
//     , connections(connections_)
//     , monastery(monastery_)
//     , pennant(pennant_) {}

   [[nodiscard]] DEVHOST inline Tile rotate(mb::size count) const {
      Contact rotatedContacts = contacts;
      Connection rotatedConnections = connections;
      for (mb::size i = 0; i < count; ++i) {
         rotatedContacts = rotate_contact(rotatedContacts);
         rotatedConnections = rotate_connection(rotatedConnections);
      }
      Tile result{
         /*edges*/ edges,
         /*field_edges*/ field_edges,
         /*amount*/ amount,
         /*contacts*/ contacts,
         /*connections*/ connections };
      result.contacts = rotatedContacts;
      result.connections = rotatedConnections;
      result.monastery = monastery;
      result.pennant = pennant;

      std::copy(edges.begin(), edges.end(), result.edges.begin());
      std::copy(field_edges.begin(), field_edges.end(), result.field_edges.begin());

      std::rotate(result.edges.begin(), result.edges.end() - count, result.edges.end());
      std::rotate(result.field_edges.begin(), result.field_edges.end() - 2 * count, result.field_edges.end());

      return result;
   }
};

DEVHOST constexpr auto g_max_moves = 71;

DEVHOST constexpr std::array<Tile, 9> g_tiles{ // 25> g_tiles{
   Tile{
      /*edges*/ std::array<EdgeType, 4>{EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass},
      /*field_edges*/ std::array<EdgeType, 8>{EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass},
   },
   Tile{
      // 1
      /*edges*/ std::array<EdgeType, 4>{EdgeType::Path, EdgeType::Town, EdgeType::Path, EdgeType::Grass},
      /*field_edges*/ std::array<EdgeType, 8>{EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass},
      /*amount*/ 3,
      /*contacts*/ Contact{Contact::EastNEY | Contact::EastSEY},
      /*connections*/ Connection{Connection::NorthSouth | Connection::EastX | Connection::West},
   },
   Tile{
      // 2
      /*edges*/ std::array<EdgeType, 4>{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass},
      /*field_edges*/ std::array<EdgeType, 8>{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Town},
      /*amount*/ 5,
      /*contacts*/ Contact{Contact::North},
      /*connections*/ Connection{Connection::EastY | Connection::South | Connection::WestY},
   },
   Tile{
      // 3
      /*edges*/ std::array<EdgeType, 4>{EdgeType::Town, EdgeType::Grass, EdgeType::Town, EdgeType::Grass},
      /*field_edges*/ std::array<EdgeType, 8>{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Town},
      /*amount*/ 1,
      /*contacts*/ Contact{Contact::NorthW | Contact::NorthE | Contact::SouthW | Contact::SouthE},
      /*connections*/ Connection{Connection::NorthSouth | Connection::EastY | Connection::WestY},
   },
   Tile{
      // 4
      /*edges*/ std::array<EdgeType, 4>{EdgeType::Grass, EdgeType::Town, EdgeType::Grass, EdgeType::Town},
      /*field_edges*/ std::array<EdgeType, 8>{EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Grass},
      /*amount*/ 3,
      /*contacts*/ Contact{Contact::EastN | Contact::EastS | Contact::WestN | Contact::WestS},
      /*connections*/ Connection{Connection::NorthX | Connection::SouthX | Connection::WestX | Connection::EastX},
   },
   Tile{
      // 5
      /*edges*/ std::array<EdgeType, 4>{EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass},
      /*field_edges*/ std::array<EdgeType, 8>{EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass},
      /*amount*/ 4,
      /*contacts*/ Contact{Contact::None},
      /*connections*/ Connection{Connection::AllFieldEdges},
      /*monastery*/ true,
   },
   Tile{
      // 6
      /*edges*/ std::array<EdgeType, 4>{EdgeType::Grass, EdgeType::Grass, EdgeType::Path, EdgeType::Grass},
      /*field_edges*/ std::array<EdgeType, 8>{EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass},
      /*amount*/ 2,
      /*contacts*/ Contact{Contact::None},
      /*connections*/ Connection{Connection::AllFieldEdges},
      /*monastery*/ true,
   },
   Tile{
      // 7
      /*edges*/ std::array<EdgeType, 4>{EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Grass},
      /*field_edges*/ std::array<EdgeType, 8>{EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass},
      /*amount*/ 2,
      /*contacts*/ Contact{Contact::SouthN | Contact::SouthW | Contact::EastW | Contact::EastN},
      /*connections*/ Connection{Connection::NorthWestCorner | Connection::NorthX | Connection::WestY},
   },
   Tile{
      // 8
      /*edges*/ std::array<EdgeType, 4>{EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Town},
      /*field_edges*/ std::array<EdgeType, 8>{EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Town},
      /*amount*/ 1,
      /*contacts*/ Contact{Contact::None},
      /*connections*/ Connection{Connection::AllEdges},
      /*monastery*/ false,
      /*pennant*/ true,
   },
        // {
        //         // 9
        //         .edges{EdgeType::Town, EdgeType::Path, EdgeType::Path, EdgeType::Grass},
        //         .field_edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Town},
        //         .contacts = Contact::NorthW | Contact::NorthNEX | Contact::NorthSWY,
        //         .connections = Connection::SouthEast | Connection::WestY | Connection::SouthWestCorner | Connection::NorthY | Connection::SouthEastCorner,
        //         .amount = 3,
        // },
        // {
        //         // 10
        //         .edges{EdgeType::Town, EdgeType::Grass, EdgeType::Path, EdgeType::Path},
        //         .field_edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Town},
        //         .contacts = Contact::NorthW | Contact::NorthNWX | Contact::NorthSEY,
        //         .connections = Connection::SouthWest | Connection::EastY | Connection::SouthWestCorner | Connection::NorthY | Connection::SouthEastCorner,
        //         .amount = 3,
        // },
        // {
        //         // 11
        //         .edges{EdgeType::Town, EdgeType::Path, EdgeType::Path, EdgeType::Path},
        //         .field_edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Town},
        //         .contacts = Contact::NorthNEX | Contact::NorthNWX,
        //         .connections = Connection::NorthY | Connection::SouthEastCorner | Connection::SouthWestCorner,
        //         .amount = 3,
        // },
        // {
        //         // 12
        //         .edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Town},
        //         .field_edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Town},
        //         .contacts = Contact::NorthE | Contact::NorthS | Contact::WestE | Contact::WestS,
        //         .connections = Connection::NorthWest | Connection::SouthX | Connection::EastY | Connection::SouthEastCorner,
        //         .pennant = true,
        //         .amount = 2,
        // },
        // {
        //         // 13
        //         .edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Town},
        //         .field_edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Town},
        //         .contacts = Contact::NorthE | Contact::NorthS | Contact::WestE | Contact::WestS,
        //         .connections = Connection::NorthWest | Connection::SouthX | Connection::EastY | Connection::SouthEastCorner,
        //         .amount = 3,
        // },
        // {
        //         // 14
        //         .edges{EdgeType::Town, EdgeType::Path, EdgeType::Path, EdgeType::Town},
        //         .field_edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Town},
        //         .contacts = Contact::NorthNEX | Contact::NorthSWY | Contact::WestNEX | Contact::WestSWY,
        //         .connections = Connection::NorthWest | Connection::SouthEast | Connection::SouthEastCorner | Connection::EastNorthCross,
        //         .amount = 3,
        // },
        // {
        //         // 15
        //         .edges{EdgeType::Town, EdgeType::Path, EdgeType::Path, EdgeType::Town},
        //         .field_edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Town},
        //         .contacts = Contact::NorthNEX | Contact::NorthSWY | Contact::WestNEX | Contact::WestSWY,
        //         .connections = Connection::NorthWest | Connection::SouthEast | Connection::SouthEastCorner | Connection::EastNorthCross,
        //         .pennant = true,
        //         .amount = 2,
        // },
        // {
        //         // 16
        //         .edges{EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Town},
        //         .field_edges{EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Town},
        //         .contacts = Contact::NorthS | Contact::WestS | Contact::EastS,
        //         .connections = Connection::NorthEast | Connection::NorthWest | Connection::WestEast | Connection::SouthX,
        //         .pennant = true,
        //         .amount = 1,
        // },
        // {
        //         // 17
        //         .edges{EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Town},
        //         .field_edges{EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Town},
        //         .contacts = Contact::NorthS | Contact::WestS | Contact::EastS,
        //         .connections = Connection::NorthEast | Connection::NorthWest | Connection::WestEast | Connection::SouthX,
        //         .amount = 3,
        // },
        // {
        //         // 18
        //         .edges{EdgeType::Town, EdgeType::Town, EdgeType::Path, EdgeType::Town},
        //         .field_edges{EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Town},
        //         .contacts = Contact::NorthS | Contact::WestS | Contact::EastS,// not sure
        //         .connections = Connection::NorthEast | Connection::NorthWest | Connection::WestEast,
        //         .pennant = true,
        //         .amount = 2,
        // },
        // {
        //         // 19
        //         .edges{EdgeType::Town, EdgeType::Town, EdgeType::Path, EdgeType::Town},
        //         .field_edges{EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Town},
        //         .contacts = Contact::NorthS | Contact::WestS | Contact::EastS,// not sure
        //         .connections = Connection::NorthEast | Connection::NorthWest | Connection::WestEast,
        //         .amount = 1,
        // },
        // {
        //         // 20
        //         .edges{EdgeType::Path, EdgeType::Grass, EdgeType::Path, EdgeType::Grass},
        //         .field_edges{EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass},
        //         .connections = Connection::NorthSouth | Connection::West | Connection::East,
        //         .amount = 8,
        // },
        // {
        //         // 21
        //         .edges{EdgeType::Grass, EdgeType::Grass, EdgeType::Path, EdgeType::Path},
        //         .field_edges{EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass},
        //         .connections = Connection::SouthWest | Connection::North | Connection::East | Connection::SouthWestCorner,
        //         .amount = 9,
        // },
        // {
        //         // 22
        //         .edges{EdgeType::Grass, EdgeType::Path, EdgeType::Path, EdgeType::Path},
        //         .field_edges{EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass},
        //         .connections = Connection::North | Connection::SouthEastCorner | Connection::SouthWestCorner,
        //         .amount = 4,
        // },
        // {
        //         // 23
        //         .edges{EdgeType::Path, EdgeType::Path, EdgeType::Path, EdgeType::Path},
        //         .field_edges{EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass},
        //         .connections = Connection::NorthEastCorner | Connection::SouthEastCorner | Connection::SouthWestCorner | Connection::NorthWestCorner,
        //         .amount = 1,
        // },
        // {
        //         // 24
        //         .edges{EdgeType::Town, EdgeType::Grass, EdgeType::Town, EdgeType::Grass},
        //         .field_edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Town},
        //         .contacts = Contact::NorthW | Contact::NorthE | Contact::SouthW | Contact::SouthE,
        //         .connections = Connection::NorthSouth | Connection::WestY | Connection::EastY,
        //         .pennant = true,
        //         .amount = 2,
        // },
};

using TileType = mb::u8;

struct TilePlacement {
   TileType type = 0;
   mb::u8 rotation = 0;

   DEVHOST [[nodiscard]] inline Tile tile() const {
      return g_tiles[type].rotate(rotation);
   }
};

DEVHOST constexpr TilePosition neighbour_of(int x, int y, Direction d) {
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

}// namespace carcassonne

#endif//CARCASSONNE_GPU_TILES_H
