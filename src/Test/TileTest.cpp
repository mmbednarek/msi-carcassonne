#include <gtest/gtest.h>
#include <Carcassonne/Core.h>

TEST(Tile, RotateBasic) {
   using carcassonne::EdgeType;
   using carcassonne::Connection;
   auto tile = carcassonne::g_tiles[1];

   // SINGLE ROTATION
   {
      carcassonne::Tile rotated = tile.rotate(1);
      std::array<EdgeType, 4> expected_edges_rotation{EdgeType::Grass, EdgeType::Path, EdgeType::Town, EdgeType::Path};
      std::array<EdgeType, 8> expected_field_edges_rotation{EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass};
      Connection expected_connection_rotation{Connection::WestEast | Connection::SouthY | Connection::North};
      ASSERT_EQ(rotated.edges, expected_edges_rotation);
      ASSERT_EQ(rotated.field_edges, expected_field_edges_rotation);
      ASSERT_EQ(rotated.connections, expected_connection_rotation);
      ASSERT_EQ(rotated.monastery, false);
      ASSERT_EQ(rotated.pennant, false);
   }
   // DOUBLE ROTATION
   {
      carcassonne::Tile rotated = tile.rotate(2);
      std::array<EdgeType, 4> expected_edges_rotation{EdgeType::Path, EdgeType::Grass, EdgeType::Path, EdgeType::Town};
      std::array<EdgeType, 8> expected_field_edges_rotation{EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Town, EdgeType::Town, EdgeType::Grass};
      Connection expected_connection_rotation{Connection::NorthSouth | Connection::WestX | Connection::East};
      ASSERT_EQ(rotated.edges, expected_edges_rotation);
      ASSERT_EQ(rotated.field_edges, expected_field_edges_rotation);
      ASSERT_EQ(rotated.connections, expected_connection_rotation);
      ASSERT_EQ(rotated.monastery, false);
      ASSERT_EQ(rotated.pennant, false);
   }
   // TRIPLE ROTATION
   {
      carcassonne::Tile rotated = tile.rotate(3);
      std::array<EdgeType, 4> expected_edges_rotation{EdgeType::Town, EdgeType::Path, EdgeType::Grass, EdgeType::Path};
      std::array<EdgeType, 8> expected_field_edges_rotation{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass, EdgeType::Town};
      Connection expected_connection_rotation{Connection::WestEast | Connection::NorthY | Connection::South};
      ASSERT_EQ(rotated.edges, expected_edges_rotation);
      ASSERT_EQ(rotated.field_edges, expected_field_edges_rotation);
      ASSERT_EQ(rotated.connections, expected_connection_rotation);
      ASSERT_EQ(rotated.monastery, false);
      ASSERT_EQ(rotated.pennant, false);
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
   Connection expected_connection_rotation{Connection::NorthEast | Connection::SouthWest | Connection::SouthWestCross | Connection::SouthWestCorner};

   ASSERT_EQ(rotated.edges, expected_edges_rotation);
   ASSERT_EQ(rotated.field_edges, expected_field_edges_rotation);
   ASSERT_EQ(rotated.connections, expected_connection_rotation);
   ASSERT_EQ(rotated.monastery, false);
   ASSERT_EQ(rotated.pennant, false);
}
