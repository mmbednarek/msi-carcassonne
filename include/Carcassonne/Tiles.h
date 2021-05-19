#ifndef MSI_CARCASSONNE_TILES_H
#define MSI_CARCASSONNE_TILES_H
#include <Carcassonne/Contact.h>
#include <Carcassonne/Connection.h>
#include <algorithm>

namespace carcassonne {

enum class EdgeType {
   Grass,
   Path,
   Town
};

struct Tile {
   std::array<EdgeType, 4> edges{};
   std::array<EdgeType, 8> field_edges{};
   Contact contacts = Contact::None;
   Connection connections = Connection::None;
   bool monastery = false;
   bool pennant = false;
   mb::u8 amount = std::numeric_limits<mb::u8>::infinity();

   [[nodiscard]] inline Tile rotate(mb::size count) const {
      Contact rotatedContacts = contacts;
      Connection rotatedConnections = connections;
      for (mb::size i = 0; i < count; ++i) {
         rotatedContacts = rotate_contact(rotatedContacts);
         rotatedConnections = rotate_connection(rotatedConnections);
      }
      Tile result{
              .contacts = rotatedContacts,
              .connections = rotatedConnections,
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
                // 1
                .edges{EdgeType::Path, EdgeType::Town, EdgeType::Path, EdgeType::Grass},
                .field_edges{EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass},
                .contacts = Contact::EastNEY | Contact::EastSEY,
                .connections = Connection::NorthSouth | Connection::EastX | Connection::West,
                .amount = 4,
        },
        {
                // 2
                .edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass},
                .field_edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Town},
                .contacts = Contact::North,
                .connections = Connection::EastY | Connection::South | Connection::WestY,
                .amount = 5,
        },
        {
                // 3
                .edges{EdgeType::Town, EdgeType::Grass, EdgeType::Town, EdgeType::Grass},
                .field_edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Town},
                .contacts = Contact::NorthW | Contact::NorthE | Contact::SouthW | Contact::SouthE,
                .connections = Connection::NorthSouth | Connection::EastY | Connection::WestY,
                .amount = 1,
        },
        {
                // 4
                .edges{EdgeType::Grass, EdgeType::Town, EdgeType::Grass, EdgeType::Town},
                .field_edges{EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Grass},
                .contacts = Contact::EastN | Contact::EastS | Contact::WestN | Contact::WestS,
                .connections = Connection::NorthX | Connection::SouthX | Connection::WestX | Connection::EastX,
                .amount = 3,
        },
        {
                // 5
                .edges{EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass},
                .field_edges{EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass},
                .connections = Connection::AllFieldEdges,
                .monastery = true,
                .amount = 4,
        },
        {
                // 6
                .edges{EdgeType::Grass, EdgeType::Grass, EdgeType::Path, EdgeType::Grass},
                .field_edges{EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass},
                .connections = Connection::AllFieldEdges,
                .monastery = true,
                .amount = 2,
        },
        {
                // 7
                .edges{EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Grass},
                .field_edges{EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass},
                .contacts = Contact::SouthN | Contact::SouthW | Contact::EastW | Contact::EastN,
                .connections = Connection::NorthWestCorner | Connection::NorthX | Connection::WestY,
                .amount = 2,
        },
        {
                // 8
                .edges{EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Town},
                .field_edges{EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Town},
                .connections = Connection::AllEdges,
                .pennant = true,
                .amount = 1,
        },
        {
                // 9
                .edges{EdgeType::Town, EdgeType::Path, EdgeType::Path, EdgeType::Grass},
                .field_edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Town},
                .contacts = Contact::NorthW | Contact::NorthNEX | Contact::NorthSWY,
                .connections = Connection::SouthEast | Connection::WestY | Connection::SouthWestCorner | Connection::NorthY | Connection::SouthEastCorner,
                .amount = 3,
        },
        {
                // 10
                .edges{EdgeType::Town, EdgeType::Grass, EdgeType::Path, EdgeType::Path},
                .field_edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Town},
                .contacts = Contact::NorthW | Contact::NorthNWX | Contact::NorthSEY,
                .connections = Connection::SouthWest | Connection::EastY | Connection::SouthWestCorner | Connection::NorthY | Connection::SouthEastCorner,
                .amount = 3,
        },
        {
                // 11
                .edges{EdgeType::Town, EdgeType::Path, EdgeType::Path, EdgeType::Path},
                .field_edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Town},
                .contacts = Contact::NorthNEX | Contact::NorthNWX,
                .connections = Connection::NorthY | Connection::SouthEastCorner | Connection::SouthWestCorner,
                .amount = 3,
        },
        {
                // 12
                .edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Town},
                .field_edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Town},
                .contacts = Contact::NorthE | Contact::NorthS | Contact::WestE | Contact::WestS,
                .connections = Connection::NorthWest | Connection::SouthX | Connection::EastY | Connection::SouthEastCorner,
                .pennant = true,
                .amount = 2,
        },
        {
                // 13
                .edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Town},
                .field_edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Town},
                .contacts = Contact::NorthE | Contact::NorthS | Contact::WestE | Contact::WestS,
                .connections = Connection::NorthWest | Connection::SouthX | Connection::EastY | Connection::SouthEastCorner,
                .amount = 3,
        },
        {
                // 14
                .edges{EdgeType::Town, EdgeType::Path, EdgeType::Path, EdgeType::Town},
                .field_edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Town},
                .contacts = Contact::NorthNEX | Contact::NorthSWY | Contact::WestNEX | Contact::WestSWY,
                .connections = Connection::NorthWest | Connection::SouthEast | Connection::SouthEastCorner | Connection::EastNorthCross,
                .amount = 3,
        },
        {
                // 15
                .edges{EdgeType::Town, EdgeType::Path, EdgeType::Path, EdgeType::Town},
                .field_edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Town},
                .contacts = Contact::NorthNEX | Contact::NorthSWY | Contact::WestNEX | Contact::WestSWY,
                .connections = Connection::NorthWest | Connection::SouthEast | Connection::SouthEastCorner | Connection::EastNorthCross,
                .pennant = true,
                .amount = 2,
        },
        {
                // 16
                .edges{EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Town},
                .field_edges{EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Town},
                .contacts = Contact::NorthS | Contact::WestS | Contact::EastS,
                .connections = Connection::NorthEast | Connection::NorthWest | Connection::WestEast | Connection::SouthX,
                .pennant = true,
                .amount = 1,
        },
        {
                // 17
                .edges{EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Town},
                .field_edges{EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Town},
                .contacts = Contact::NorthS | Contact::WestS | Contact::EastS,
                .connections = Connection::NorthEast | Connection::NorthWest | Connection::WestEast | Connection::SouthX,
                .amount = 3,
        },
        {
                // 18
                .edges{EdgeType::Town, EdgeType::Town, EdgeType::Path, EdgeType::Town},
                .field_edges{EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Town},
                .contacts = Contact::NorthS | Contact::WestS | Contact::EastS,// not sure
                .connections = Connection::NorthEast | Connection::NorthWest | Connection::WestEast,
                .pennant = true,
                .amount = 2,
        },
        {
                // 19
                .edges{EdgeType::Town, EdgeType::Town, EdgeType::Path, EdgeType::Town},
                .field_edges{EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Town},
                .contacts = Contact::NorthS | Contact::WestS | Contact::EastS,// not sure
                .connections = Connection::NorthEast | Connection::NorthWest | Connection::WestEast,
                .amount = 1,
        },
        {
                // 20
                .edges{EdgeType::Path, EdgeType::Grass, EdgeType::Path, EdgeType::Grass},
                .field_edges{EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass},
                .connections = Connection::NorthSouth | Connection::West | Connection::East,
                .amount = 8,
        },
        {
                // 21
                .edges{EdgeType::Grass, EdgeType::Grass, EdgeType::Path, EdgeType::Path},
                .field_edges{EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass},
                .connections = Connection::SouthWest | Connection::North | Connection::East | Connection::SouthWestCorner,
                .amount = 9,
        },
        {
                // 22
                .edges{EdgeType::Grass, EdgeType::Path, EdgeType::Path, EdgeType::Path},
                .field_edges{EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass},
                .connections = Connection::North | Connection::SouthEastCorner | Connection::SouthWestCorner,
                .amount = 4,
        },
        {
                // 23
                .edges{EdgeType::Path, EdgeType::Path, EdgeType::Path, EdgeType::Path},
                .field_edges{EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass},
                .connections = Connection::NorthEastCorner | Connection::SouthEastCorner | Connection::SouthWestCorner | Connection::NorthWestCorner,
                .amount = 1,
        },
        {
                // 24
                .edges{EdgeType::Town, EdgeType::Grass, EdgeType::Town, EdgeType::Grass},
                .field_edges{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Town},
                .contacts = Contact::NorthW | Contact::NorthE | Contact::SouthW | Contact::SouthE,
                .connections = Connection::NorthSouth | Connection::WestY | Connection::EastY,
                .pennant = true,
                .amount = 2,
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

}// namespace carcassonne

#endif//MSI_CARCASSONNE_TILES_H
