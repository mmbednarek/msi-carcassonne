#include <gtest/gtest.h>
#include <Carcassonne/Core.h>

TEST(Tile, RotateBasic) {
   using carcassonne::EdgeType;
   using carcassonne::Connection;
   auto tile = carcassonne::g_tiles[1];

   // SINGLE ROTATION
   {
      carcassonne::Tile rotated = tile.rotate(1);
      std::array<EdgeType, 4> expected_rotation{EdgeType::Grass, EdgeType::Path, EdgeType::Town, EdgeType::Path};
      ASSERT_EQ(rotated.edges, expected_rotation);
      ASSERT_EQ(rotated.connections, Connection::WestEast);
      ASSERT_EQ(rotated.monastery, false);
      ASSERT_EQ(rotated.pennant, false);
   }
   // DOUBLE ROTATION
   {
      carcassonne::Tile rotated = tile.rotate(2);
      std::array<EdgeType, 4> expected_rotation{EdgeType::Path, EdgeType::Grass, EdgeType::Path, EdgeType::Town};
      ASSERT_EQ(rotated.edges, expected_rotation);
      ASSERT_EQ(rotated.connections, Connection::NorthSouth);
      ASSERT_EQ(rotated.monastery, false);
      ASSERT_EQ(rotated.pennant, false);
   }
   // TRIPLE ROTATION
   {
      carcassonne::Tile rotated = tile.rotate(3);
      std::array<EdgeType, 4> expected_rotation{EdgeType::Town, EdgeType::Path, EdgeType::Grass, EdgeType::Path};
      ASSERT_EQ(rotated.edges, expected_rotation);
      ASSERT_EQ(rotated.connections, Connection::WestEast);
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
   using carcassonne::CornerType;
   using carcassonne::Connection;
   auto tile = carcassonne::g_tiles[14];

   carcassonne::Tile rotated = tile.rotate(1);
   std::array<EdgeType, 4> expected_edges_rotation{EdgeType::Town, EdgeType::Town, EdgeType::Path, EdgeType::Path};
   std::array<EdgeType, 4> expected_corners_rotation{EdgeType::Town, EdgeType::Grass, EdgeType::Grass, EdgeType::Grass};
   ASSERT_EQ(rotated.edges, expected_edges_rotation);
   ASSERT_EQ(rotated.connections, Connection::NorthEast | Connection::SouthWest);
   ASSERT_EQ(rotated.monastery, false);
   ASSERT_EQ(rotated.pennant, false);
}
