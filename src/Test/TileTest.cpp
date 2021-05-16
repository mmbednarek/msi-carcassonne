#include <gtest/gtest.h>
#include <Carcassonne/Core.h>

TEST(Tile, RotateBasic) {
   using carcassonne::EdgeType;
   using carcassonne::Contact;
   using carcassonne::Connection;
   auto tile = carcassonne::g_tiles[15];

   // SINGLE ROTATION
   {
      carcassonne::Tile rotated = tile.rotate(1);
      std::array<EdgeType, 4> expected_edges_rotation{EdgeType::Town, EdgeType::Town, EdgeType::Path, EdgeType::Path};
      std::array<EdgeType, 8> expected_field_edges_rotation{EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Town};
      Contact expected_contact_rotation{Contact::EastSEY | Contact::EastNWX | Contact::NorthSEY | Contact::NorthNWX};
      Connection expected_connection_rotation{Connection::NorthEast | Connection::SouthWest | Connection::SouthWestCorner | Connection::WestSouthCross};
      ASSERT_EQ(rotated.edges, expected_edges_rotation);
      ASSERT_EQ(rotated.field_edges, expected_field_edges_rotation);
      ASSERT_EQ(rotated.contacts, expected_contact_rotation);
      ASSERT_EQ(rotated.connections, expected_connection_rotation);
      ASSERT_EQ(rotated.monastery, false);
      ASSERT_EQ(rotated.pennant, true);
   }
   // DOUBLE ROTATION
   {
      carcassonne::Tile rotated = tile.rotate(2);
      std::array<EdgeType, 4> expected_edges_rotation{EdgeType::Path, EdgeType::Town, EdgeType::Town, EdgeType::Path};
      std::array<EdgeType, 8> expected_field_edges_rotation{EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass};
      Contact expected_contact_rotation{Contact::SouthSWX | Contact::SouthNEY | Contact::EastSWX | Contact::EastNEY};
      Connection expected_connection_rotation{Connection::SouthEast | Connection::NorthWest | Connection::NorthWestCorner | Connection::NorthWestCross};
      ASSERT_EQ(rotated.edges, expected_edges_rotation);
      ASSERT_EQ(rotated.field_edges, expected_field_edges_rotation);
      ASSERT_EQ(rotated.contacts, expected_contact_rotation);
      ASSERT_EQ(rotated.connections, expected_connection_rotation);
      ASSERT_EQ(rotated.monastery, false);
      ASSERT_EQ(rotated.pennant, true);
   }
   // TRIPLE ROTATION
   {
      carcassonne::Tile rotated = tile.rotate(3);
      std::array<EdgeType, 4> expected_edges_rotation{EdgeType::Path, EdgeType::Path, EdgeType::Town, EdgeType::Town};
      std::array<EdgeType, 8> expected_field_edges_rotation{EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Grass};
      Contact expected_contact_rotation{Contact::WestNWY | Contact::WestSEX | Contact::SouthNWY | Contact::SouthSEX};
      Connection expected_connection_rotation{Connection::SouthWest | Connection::NorthEast | Connection::NorthEastCorner | Connection::EastNorthCross};
      ASSERT_EQ(rotated.edges, expected_edges_rotation);
      ASSERT_EQ(rotated.field_edges, expected_field_edges_rotation);
      ASSERT_EQ(rotated.contacts, expected_contact_rotation);
      ASSERT_EQ(rotated.connections, expected_connection_rotation);
      ASSERT_EQ(rotated.monastery, false);
      ASSERT_EQ(rotated.pennant, true);
   }
}

TEST(Tile, RotateAllConnection) {
   using carcassonne::EdgeType;
   using carcassonne::Connection;
   auto tile = carcassonne::g_tiles[8];

   for (int i = 0; i < 4; ++i) {
      carcassonne::Tile rotated = tile.rotate(1);
      std::array<EdgeType, 4> expected_rotation{EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Town};
      ASSERT_EQ(rotated.edges, expected_rotation);
      ASSERT_EQ(rotated.connections, Connection::AllEdges);
      ASSERT_EQ(rotated.monastery, false);
      ASSERT_EQ(rotated.pennant, true);
   }
}

TEST(Tile, Rotate4Connections) {
   using carcassonne::EdgeType;
   using carcassonne::EdgeType;
   using carcassonne::Connection;
   auto tile = carcassonne::g_tiles[14];

   carcassonne::Tile rotated = tile.rotate(1);
   std::array<EdgeType, 4> expected_edges_rotation{EdgeType::Town, EdgeType::Town, EdgeType::Path, EdgeType::Path};
   std::array<EdgeType, 8> expected_field_edges_rotation{EdgeType::Town, EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Town};
   Connection expected_connection_rotation{Connection::NorthEast | Connection::SouthWest | Connection::WestSouthCross | Connection::SouthWestCorner};

   ASSERT_EQ(rotated.edges, expected_edges_rotation);
   ASSERT_EQ(rotated.field_edges, expected_field_edges_rotation);
   ASSERT_EQ(rotated.connections, expected_connection_rotation);
   ASSERT_EQ(rotated.monastery, false);
   ASSERT_EQ(rotated.pennant, false);
}
