#include <gtest/gtest.h>
#include <Carcassonne/Core.h>
#include <Carcassonne/Game/Game.h>
#include <fmt/core.h>

void assert_same_group(carcassonne::game::Game &g, const std::vector<carcassonne::Edge> &vec) {
   auto beg = vec.begin();
   auto end = vec.end();
   if (beg == end)
      return;

   auto group = g.groups().group_of(*beg);
   ASSERT_TRUE(std::all_of(beg+1, end, [&g, group](carcassonne::Edge e) { return g.groups().group_of(e) == group; }));
}

TEST(Board, Basic) {
   carcassonne::game::Game g;
   g.apply_tile(71, 70, 1, 3);
   g.apply_tile(70, 69, 2, 2);
   g.apply_tile(71, 69, 2, 2);

   {
      auto path1 = carcassonne::make_edge(70, 70, carcassonne::Direction::West);
      auto path2 = carcassonne::make_edge(70, 70, carcassonne::Direction::East);
      auto path3 = carcassonne::make_edge(71, 70, carcassonne::Direction::West);
      auto path4 = carcassonne::make_edge(71, 70, carcassonne::Direction::East);
      ASSERT_EQ(path2, path3);
      assert_same_group(g, {path1, path2, path3, path4});
   }
   {
      auto path1 = carcassonne::make_edge(70, 70, carcassonne::Direction::WestNorth);
      auto path2 = carcassonne::make_edge(70, 70, carcassonne::Direction::EastNorth);
      auto path3 = carcassonne::make_edge(71, 70, carcassonne::Direction::WestNorth);
      auto path4 = carcassonne::make_edge(71, 70, carcassonne::Direction::EastNorth);
      ASSERT_NE(path1, path2);
      ASSERT_EQ(path2, path3);
      ASSERT_NE(path3, path4);
      assert_same_group(g, {path1, path2, path3, path4});
   }
   {
      auto path1 = carcassonne::make_edge(70, 70, carcassonne::Direction::WestSouth);
      auto path2 = carcassonne::make_edge(70, 70, carcassonne::Direction::EastSouth);
      auto path3 = carcassonne::make_edge(71, 70, carcassonne::Direction::WestSouth);
      auto path4 = carcassonne::make_edge(71, 70, carcassonne::Direction::EastSouth);
      auto path5 = carcassonne::make_edge(70, 70, carcassonne::Direction::SouthWest);
      auto path6 = carcassonne::make_edge(70, 70, carcassonne::Direction::SouthEast);
      auto path7 = carcassonne::make_edge(71, 70, carcassonne::Direction::SouthWest);
      auto path8 = carcassonne::make_edge(71, 70, carcassonne::Direction::SouthEast);
      auto pathAbove = carcassonne::make_edge(71, 69, carcassonne::Direction::WestSouth);
      ASSERT_NE(path1, path2);
      ASSERT_EQ(path2, path3);
      ASSERT_NE(path3, path4);
      ASSERT_NE(path5, path6);
      ASSERT_NE(path6, path7);
      ASSERT_NE(path7, path8);
      ASSERT_NE(g.groups().group_of(path7), g.groups().group_of(pathAbove));
      assert_same_group(g, {path1, path2, path3, path4, path5, path6, path7, path8});
   }
   {
      auto path1 = carcassonne::make_edge(70, 69, carcassonne::Direction::NorthWest);
      auto path2 = carcassonne::make_edge(70, 69, carcassonne::Direction::NorthEast);
      auto path3 = carcassonne::make_edge(71, 69, carcassonne::Direction::NorthWest);
      auto path4 = carcassonne::make_edge(71, 69, carcassonne::Direction::NorthEast);
      auto path5 = carcassonne::make_edge(70, 69, carcassonne::Direction::WestNorth);
      auto path6 = carcassonne::make_edge(70, 69, carcassonne::Direction::EastNorth);
      auto path7 = carcassonne::make_edge(71, 69, carcassonne::Direction::WestNorth);
      auto path8 = carcassonne::make_edge(71, 69, carcassonne::Direction::EastNorth);
      auto path9 = carcassonne::make_edge(70, 69, carcassonne::Direction::WestSouth);
      auto path10 = carcassonne::make_edge(70, 69, carcassonne::Direction::EastSouth);
      auto path11 = carcassonne::make_edge(71, 69, carcassonne::Direction::WestSouth);
      auto path12 = carcassonne::make_edge(71, 69, carcassonne::Direction::EastSouth);
      auto pathBelow = carcassonne::make_edge(70, 70, carcassonne::Direction::WestSouth);
      ASSERT_NE(g.groups().group_of(path7), g.groups().group_of(pathBelow));
      ASSERT_NE(path2, path3);
      ASSERT_EQ(path6, path7);
      ASSERT_EQ(path10, path11);
      assert_same_group(g, {path1, path2, path3, path4,
                            path5, path6, path7, path8,
                            path9, path10, path11, path12});
   }
   {
      auto path1 = carcassonne::make_edge(70, 70, carcassonne::Direction::North);
      auto path2 = carcassonne::make_edge(70, 69, carcassonne::Direction::South);
      auto pathEast = carcassonne::make_edge(71, 69, carcassonne::Direction::South);
      ASSERT_EQ(path1, path2);
      ASSERT_NE(g.groups().group_of(path2), g.groups().group_of(pathEast));
   }
}

TEST(Board, Path) {
   carcassonne::game::Game g;
   g.apply_tile(71, 70, 9, 1);
   g.apply_tile(72, 70, 4, 0);
   g.apply_tile(71, 71, 23, 0);
   g.apply_tile(72, 71, 21, 0);

   auto edge1 = carcassonne::make_edge(70, 70, carcassonne::Direction::WestNorth);
   auto edge2 = carcassonne::make_edge(71, 70, carcassonne::Direction::NorthWest);
   auto edge3 = carcassonne::make_edge(72, 70, carcassonne::Direction::NorthWest);
   auto edge4 = carcassonne::make_edge(72, 70, carcassonne::Direction::SouthWest);
   auto edge5 = carcassonne::make_edge(72, 71, carcassonne::Direction::NorthWest);
   auto edge6 = carcassonne::make_edge(72, 71, carcassonne::Direction::SouthEast);
   auto edgeDis1 = carcassonne::make_edge(72, 71, carcassonne::Direction::SouthWest);
   auto edgeDis2 = carcassonne::make_edge(71, 71, carcassonne::Direction::SouthWest);
   ASSERT_EQ(edge4, edge5);
   assert_same_group(g, {edge1, edge2, edge3, edge4, edge5, edge6});
   ASSERT_NE(g.groups().group_of(edge1), g.groups().group_of(edgeDis1));
   ASSERT_NE(g.groups().group_of(edge1), g.groups().group_of(edgeDis2));
   ASSERT_NE(g.groups().group_of(edgeDis1), g.groups().group_of(edgeDis2));
}