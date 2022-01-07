#include <Carcassonne/IGame.h>
#include <Carcassonne/Game/Game.h>
#include <fmt/core.h>
#include <gtest/gtest.h>

void assert_same_group(carcassonne::game::Game &g, const std::vector<carcassonne::Edge> &vec) {
   auto beg = vec.begin();
   auto end = vec.end();
   if (beg == end)
      return;

   auto group = g.groups().group_of(*beg);
   ASSERT_TRUE(std::all_of(beg + 1, end, [&g, group](carcassonne::Edge e) { return g.groups().group_of(e) == group; }));
}

TEST(Board, Basic) {
   carcassonne::game::Game g(4, 0);
   g.apply_tile(21, 20, 1, 3);
   g.apply_tile(20, 19, 2, 2);
   g.apply_tile(21, 19, 2, 2);

   {
      auto path1 = carcassonne::make_edge(20, 20, carcassonne::Direction::West);
      auto path2 = carcassonne::make_edge(20, 20, carcassonne::Direction::East);
      auto path3 = carcassonne::make_edge(21, 20, carcassonne::Direction::West);
      auto path4 = carcassonne::make_edge(21, 20, carcassonne::Direction::East);
      ASSERT_EQ(path2, path3);
      assert_same_group(g, {path1, path2, path3, path4});
      ASSERT_EQ(g.groups().type_of(path1), carcassonne::EdgeType::Path);
   }
   {
      auto path1 = carcassonne::make_edge(20, 20, carcassonne::Direction::WestNorth);
      auto path2 = carcassonne::make_edge(20, 20, carcassonne::Direction::EastNorth);
      auto path3 = carcassonne::make_edge(21, 20, carcassonne::Direction::WestNorth);
      auto path4 = carcassonne::make_edge(21, 20, carcassonne::Direction::EastNorth);
      ASSERT_NE(path1, path2);
      ASSERT_EQ(path2, path3);
      ASSERT_NE(path3, path4);
      assert_same_group(g, {path1, path2, path3, path4});
      ASSERT_EQ(g.groups().type_of(path1), carcassonne::EdgeType::Grass);
   }
   {
      auto path1 = carcassonne::make_edge(20, 20, carcassonne::Direction::WestSouth);
      auto path2 = carcassonne::make_edge(20, 20, carcassonne::Direction::EastSouth);
      auto path3 = carcassonne::make_edge(21, 20, carcassonne::Direction::WestSouth);
      auto path4 = carcassonne::make_edge(21, 20, carcassonne::Direction::EastSouth);
      auto path5 = carcassonne::make_edge(20, 20, carcassonne::Direction::SouthWest);
      auto path6 = carcassonne::make_edge(20, 20, carcassonne::Direction::SouthEast);
      auto path7 = carcassonne::make_edge(21, 20, carcassonne::Direction::SouthWest);
      auto path8 = carcassonne::make_edge(21, 20, carcassonne::Direction::SouthEast);
      auto pathAbove = carcassonne::make_edge(21, 19, carcassonne::Direction::WestSouth);
      ASSERT_NE(path1, path2);
      ASSERT_EQ(path2, path3);
      ASSERT_NE(path3, path4);
      ASSERT_NE(path5, path6);
      ASSERT_NE(path6, path7);
      ASSERT_NE(path7, path8);
      ASSERT_NE(g.groups().group_of(path7), g.groups().group_of(pathAbove));
      assert_same_group(g, {path1, path2, path3, path4, path5, path6, path7, path8});
      ASSERT_EQ(g.groups().type_of(path1), carcassonne::EdgeType::Grass);
   }
   {
      auto path1 = carcassonne::make_edge(20, 19, carcassonne::Direction::NorthWest);
      auto path2 = carcassonne::make_edge(20, 19, carcassonne::Direction::NorthEast);
      auto path3 = carcassonne::make_edge(21, 19, carcassonne::Direction::NorthWest);
      auto path4 = carcassonne::make_edge(21, 19, carcassonne::Direction::NorthEast);
      auto path5 = carcassonne::make_edge(20, 19, carcassonne::Direction::WestNorth);
      auto path6 = carcassonne::make_edge(20, 19, carcassonne::Direction::EastNorth);
      auto path7 = carcassonne::make_edge(21, 19, carcassonne::Direction::WestNorth);
      auto path8 = carcassonne::make_edge(21, 19, carcassonne::Direction::EastNorth);
      auto path9 = carcassonne::make_edge(20, 19, carcassonne::Direction::WestSouth);
      auto path10 = carcassonne::make_edge(20, 19, carcassonne::Direction::EastSouth);
      auto path11 = carcassonne::make_edge(21, 19, carcassonne::Direction::WestSouth);
      auto path12 = carcassonne::make_edge(21, 19, carcassonne::Direction::EastSouth);
      auto pathBelow = carcassonne::make_edge(20, 20, carcassonne::Direction::WestSouth);
      ASSERT_NE(g.groups().group_of(path7), g.groups().group_of(pathBelow));
      ASSERT_NE(path2, path3);
      ASSERT_EQ(path6, path7);
      ASSERT_EQ(path10, path11);
      assert_same_group(g, {path1, path2, path3, path4,
                            path5, path6, path7, path8,
                            path9, path10, path11, path12});
      ASSERT_EQ(g.groups().type_of(path1), carcassonne::EdgeType::Grass);
   }
   {
      auto path1 = carcassonne::make_edge(20, 20, carcassonne::Direction::North);
      auto path2 = carcassonne::make_edge(20, 19, carcassonne::Direction::South);
      auto pathEast = carcassonne::make_edge(21, 19, carcassonne::Direction::South);
      ASSERT_EQ(path1, path2);
      ASSERT_NE(g.groups().group_of(path2), g.groups().group_of(pathEast));
      ASSERT_EQ(g.groups().type_of(path1), carcassonne::EdgeType::Town);
   }
}

TEST(Board, Path) {
   carcassonne::game::Game g(0, 0);
   g.apply_tile(21, 20, 9, 1);
   auto edgeTown1 = carcassonne::make_edge(21, 20, carcassonne::Direction::EastNorth);
   ASSERT_EQ(g.groups().type_of(edgeTown1), carcassonne::EdgeType::Town);
   g.apply_tile(22, 20, 4, 0);
   g.apply_tile(21, 21, 23, 0);
   g.apply_tile(22, 21, 21, 0);

   auto edge1 = carcassonne::make_edge(20, 20, carcassonne::Direction::WestNorth);
   auto edge2 = carcassonne::make_edge(21, 20, carcassonne::Direction::NorthWest);
   auto edge3 = carcassonne::make_edge(22, 20, carcassonne::Direction::NorthWest);
   auto edge4 = carcassonne::make_edge(22, 20, carcassonne::Direction::SouthWest);
   auto edge5 = carcassonne::make_edge(22, 21, carcassonne::Direction::NorthWest);
   auto edge6 = carcassonne::make_edge(22, 21, carcassonne::Direction::SouthEast);
   auto edgeDis1 = carcassonne::make_edge(22, 21, carcassonne::Direction::SouthWest);
   auto edgeDis2 = carcassonne::make_edge(21, 21, carcassonne::Direction::SouthWest);
   ASSERT_EQ(edge4, edge5);
   assert_same_group(g, {edge1, edge2, edge3, edge4, edge5, edge6});
   ASSERT_NE(g.groups().group_of(edge1), g.groups().group_of(edgeDis1));
   ASSERT_NE(g.groups().group_of(edge1), g.groups().group_of(edgeDis2));
   ASSERT_NE(g.groups().group_of(edgeDis1), g.groups().group_of(edgeDis2));
}

TEST(Board, FreeEdges) {
   carcassonne::game::Game g(4, 0);
   ASSERT_EQ(g.groups().free_edges(carcassonne::make_edge(20, 20, carcassonne::Direction::East)), 2);
   ASSERT_EQ(g.groups().free_edges(carcassonne::make_edge(20, 20, carcassonne::Direction::North)), 1);
   ASSERT_EQ(g.groups().tile_count(carcassonne::make_edge(20, 20, carcassonne::Direction::East)), 1);
   ASSERT_EQ(g.groups().tile_count(carcassonne::make_edge(20, 20, carcassonne::Direction::North)), 1);
   g.apply_tile(22, 20, 6, 1);
   ASSERT_EQ(g.groups().free_edges(carcassonne::make_edge(22, 20, carcassonne::Direction::West)), 1);
   ASSERT_EQ(g.groups().free_edges(carcassonne::make_edge(20, 20, carcassonne::Direction::North)), 1);
   ASSERT_EQ(g.groups().tile_count(carcassonne::make_edge(22, 20, carcassonne::Direction::West)), 1);
   ASSERT_EQ(g.groups().tile_count(carcassonne::make_edge(20, 20, carcassonne::Direction::North)), 1);
   g.apply_tile(21, 20, 20, 1);
   ASSERT_EQ(g.groups().free_edges(carcassonne::make_edge(21, 20, carcassonne::Direction::East)), 1);
   ASSERT_EQ(g.groups().tile_count(carcassonne::make_edge(21, 20, carcassonne::Direction::East)), 3);
   g.apply_tile(19, 20, 22, 0);
   ASSERT_EQ(g.groups().free_edges(carcassonne::make_edge(19, 20, carcassonne::Direction::East)), 0);
   ASSERT_EQ(g.groups().tile_count(carcassonne::make_edge(19, 20, carcassonne::Direction::East)), 4);
   ASSERT_TRUE(g.groups().is_completed(carcassonne::make_edge(19, 20, carcassonne::Direction::East)));
   ASSERT_EQ(g.groups().free_edges(carcassonne::make_edge(19, 20, carcassonne::Direction::West)), 1);
   ASSERT_EQ(g.groups().free_edges(carcassonne::make_edge(19, 20, carcassonne::Direction::South)), 1);
   g.apply_tile(20, 19, 17, 2);
   ASSERT_EQ(g.groups().free_edges(carcassonne::make_edge(20, 19, carcassonne::Direction::East)), 2);
   g.apply_tile(21, 19, 2, 3);
   ASSERT_EQ(g.groups().free_edges(carcassonne::make_edge(21, 19, carcassonne::Direction::West)), 1);
   g.apply_tile(19, 19, 2, 1);
   ASSERT_EQ(g.groups().free_edges(carcassonne::make_edge(19, 19, carcassonne::Direction::East)), 0);
   ASSERT_EQ(g.groups().tile_count(carcassonne::make_edge(19, 19, carcassonne::Direction::East)), 4);
   ASSERT_TRUE(g.groups().is_completed(carcassonne::make_edge(19, 19, carcassonne::Direction::East)));
}

TEST(Board, FieldTown) {
   carcassonne::game::Game g(0, 0);
   g.apply_tile(20, 19, 2, 2);

   ASSERT_FALSE(g.is_town_field_connected(carcassonne::make_edge(20, 20, carcassonne::Direction::North), carcassonne::make_edge(20, 20, carcassonne::Direction::SouthWest)));
   ASSERT_TRUE(g.is_town_field_connected(carcassonne::make_edge(20, 20, carcassonne::Direction::North), carcassonne::make_edge(20, 20, carcassonne::Direction::EastNorth)));
}

TEST(Board, CanPlace) {
   carcassonne::game::Game g(0, 0);
   ASSERT_TRUE(g.can_place(8));
   g.apply_tile(21, 20, 1, 3);
   ASSERT_TRUE(g.can_place(8));
   g.apply_tile(20, 19, 2, 2);
   ASSERT_FALSE(g.can_place(8));
   g.apply_tile(21, 19, 2, 2);
   ASSERT_FALSE(g.can_place(8));
   g.apply_tile(22, 19, 7, 0);
   ASSERT_TRUE(g.can_place(8));
   g.apply_tile(23, 19, 2, 3);
   ASSERT_FALSE(g.can_place(8));
   g.apply_tile(19, 19, 7, 1);
   ASSERT_TRUE(g.can_place(8));
   g.apply_tile(18, 19, 2, 1);
   ASSERT_FALSE(g.can_place(8));
   g.apply_tile(20, 18, 1, 3);
   ASSERT_TRUE(g.can_place(8));
   g.apply_tile(20, 17, 2, 2);
   ASSERT_FALSE(g.can_place(8));
   g.apply_tile(20, 21, 1, 1);
   ASSERT_TRUE(g.can_place(8));
}

TEST(Board, PossibleMoves) {
   using carcassonne::TileMove;
   using mb::u8;

   carcassonne::game::Game g(0, 0);
   carcassonne::game::TileSet ts;

   {
      const auto move_rage = g.moves(8);
      std::vector<TileMove> moves(move_rage.begin(), move_rage.end());
      std::vector<TileMove> expected_moves{
              TileMove{20, 19, 0},
              TileMove{20, 19, 1},
              TileMove{20, 19, 2},
              TileMove{20, 19, 3},
      };
      ASSERT_EQ(moves, expected_moves);
   }

   g.apply_tile(21, 20, 1, 3);
   {
      const auto move_rage = g.moves(8);
      std::vector<TileMove> moves(move_rage.begin(), move_rage.end());

      u8 index = 0;
      for (int x = 20; x <= 21; x++) {
         for (u8 rotation = 0; rotation <= 3; rotation++) {
            TileMove expected_move{x, 19, rotation};
            ASSERT_EQ(moves[index], expected_move);
            ++index;
         }
      }
   }

   g.apply_tile(20, 19, 2, 2);
   {
      const auto move_rage = g.moves(8);
      ASSERT_EQ(move_rage.begin(), move_rage.end());
   }

   g.apply_tile(21, 19, 2, 2);
   {
      const auto move_rage = g.moves(8);
      ASSERT_EQ(move_rage.begin(), move_rage.end());
   }

   g.apply_tile(22, 19, 7, 0);
   {
      const auto move_rage = g.moves(8);
      std::vector<TileMove> moves(move_rage.begin(), move_rage.end());
      auto index = 0;
      for (u8 rotation = 0; rotation <= 3; rotation++) {
         TileMove expected_move{23, 19, rotation};
         ASSERT_EQ(moves[index], expected_move);
         ++index;
      }
   }

   g.apply_tile(23, 19, 2, 3);
   {
      const auto move_rage = g.moves(8);
      ASSERT_EQ(move_rage.begin(), move_rage.end());
   }

   g.apply_tile(19, 19, 7, 1);
   {
      const auto move_rage = g.moves(8);
      std::vector<TileMove> moves(move_rage.begin(), move_rage.end());
      auto index = 0;
      for (u8 rotation = 0; rotation <= 3; rotation++) {
         TileMove expected_move{18, 19, rotation};
         ASSERT_EQ(moves[index], expected_move);
         ++index;
      }
   }

   g.apply_tile(18, 19, 2, 1);
   {
      const auto move_rage = g.moves(8);
      ASSERT_EQ(move_rage.begin(), move_rage.end());
   }

   g.apply_tile(20, 18, 1, 3);
   {
      const auto move_rage = g.moves(8);
      std::vector<TileMove> moves(move_rage.begin(), move_rage.end());
      auto index = 0;
      for (u8 rotation = 0; rotation <= 3; rotation++) {
         TileMove expected_move{20, 17, rotation};
         ASSERT_EQ(moves[index], expected_move);
         index++;
      }
   }

   g.apply_tile(20, 17, 2, 2);
   {
      const auto move_rage = g.moves(8);
      ASSERT_EQ(move_rage.begin(), move_rage.end());
   }

   g.apply_tile(20, 21, 1, 1);
   {

      const auto move_rage = g.moves(8);
      std::vector<TileMove> moves(move_rage.begin(), move_rage.end());
      auto index = 0;
      for (u8 rotation = 0; rotation <= 3; rotation++) {
         TileMove expected_move{20, 22, rotation};
         ASSERT_EQ(moves[index], expected_move);
         index++;
      }
   }
}

TEST(Board, PossibleMoves2) {
   carcassonne::game::Game g(0, 0);
   auto move_range = g.moves(22);
   std::vector<carcassonne::TileMove> moves(move_range.begin(), move_range.end());
   std::vector<carcassonne::TileMove> expected_moves{
           carcassonne::TileMove{19, 20, 0},
           carcassonne::TileMove{19, 20, 2},
           carcassonne::TileMove{19, 20, 3},
           carcassonne::TileMove{21, 20, 0},
           carcassonne::TileMove{21, 20, 1},
           carcassonne::TileMove{21, 20, 2},
           carcassonne::TileMove{20, 21, 0},
   };
   ASSERT_EQ(moves, expected_moves);
}

#define NO_ERR(stmt) ASSERT_TRUE((stmt).ok())

TEST(Board, CheckingBothAtTheSameTime) {
   using carcassonne::Player;
   using carcassonne::Direction;

   carcassonne::game::Game g(2, 0);

//   for (auto tile : g.tile_set()) {
//      std::cout << static_cast<int>(tile) << "\n";
//   }

   ASSERT_TRUE(g.can_place_tile_and_figure(20, 19, 1, 4, Direction::South));
   ASSERT_TRUE(g.can_place_tile_and_figure(20, 20, 3, 1, Direction::East));
   ASSERT_TRUE(g.can_place_tile_and_figure(21, 20, 1, 20, Direction::East));
   ASSERT_TRUE(g.can_place_tile_and_figure(20, 20, 3, 1, Direction::SouthEast));

   auto move = g.new_move(Player::Black);
   NO_ERR(move->place_tile_at(20, 19, 1));
   NO_ERR(move->place_figure(Direction::South));

   ASSERT_FALSE(g.can_place_tile_and_figure(20, 19, 1, 4, Direction::South));
   ASSERT_TRUE(g.can_place_tile_and_figure(20, 20, 3, 1, Direction::East));
   ASSERT_TRUE(g.can_place_tile_and_figure(21, 20, 1, 20, Direction::East));
   ASSERT_TRUE(g.can_place_tile_and_figure(20, 20, 3, 1, Direction::SouthEast));

   move = g.new_move(Player::Blue);
   NO_ERR(move->place_tile_at(21, 20, 1));
   NO_ERR(move->place_figure(Direction::East));

   ASSERT_FALSE(g.can_place_tile_and_figure(20, 19, 1, 4, Direction::South));
   ASSERT_FALSE(g.can_place_tile_and_figure(20, 20, 3, 1, Direction::East));
   ASSERT_FALSE(g.can_place_tile_and_figure(21, 20, 1, 20, Direction::East));
   ASSERT_FALSE(g.can_place_tile_and_figure(22, 20, 1, 20, Direction::East));
   ASSERT_TRUE(g.can_place_tile_and_figure(20, 20, 3, 1, Direction::SouthEast));
   ASSERT_TRUE(g.can_place_tile_and_figure(20, 21, 0, 5, Direction::EastNorth));
   ASSERT_TRUE(g.can_place_tile_and_figure(20, 18, 0, 8, Direction::North));

   move = g.new_move(Player::Black);
   NO_ERR(move->place_tile_at(20, 18, 0));
   NO_ERR(move->place_figure(Direction::North));

   ASSERT_FALSE(g.can_place_tile_and_figure(20, 19, 1, 4, Direction::South));
   ASSERT_FALSE(g.can_place_tile_and_figure(20, 20, 3, 1, Direction::East));
   ASSERT_FALSE(g.can_place_tile_and_figure(21, 20, 1, 20, Direction::East));
   ASSERT_FALSE(g.can_place_tile_and_figure(22, 20, 1, 20, Direction::East));
   ASSERT_TRUE(g.can_place_tile_and_figure(20, 20, 3, 1, Direction::SouthEast));
   ASSERT_TRUE(g.can_place_tile_and_figure(20, 21, 0, 5, Direction::EastNorth));
   ASSERT_FALSE(g.can_place_tile_and_figure(20, 18, 0, 8, Direction::North));
   ASSERT_FALSE(g.can_place_tile_and_figure(20, 17, 2, 13, Direction::East));
   ASSERT_FALSE(g.can_place_tile_and_figure(21, 18, 2, 7, Direction::West));
   ASSERT_TRUE(g.can_place_tile_and_figure(21, 18, 2, 7, Direction::North));
   ASSERT_TRUE(g.can_place_tile_and_figure(21, 18, 2, 7, Direction::SouthEast));

   move = g.new_move(Player::Blue);
   NO_ERR(move->place_tile_at(21, 18, 2));
   NO_ERR(move->place_figure(Direction::SouthEast));

   ASSERT_FALSE(g.can_place_tile_and_figure(20, 19, 1, 4, Direction::South));
   ASSERT_FALSE(g.can_place_tile_and_figure(20, 20, 3, 1, Direction::East));
   ASSERT_FALSE(g.can_place_tile_and_figure(21, 20, 1, 20, Direction::East));
   ASSERT_FALSE(g.can_place_tile_and_figure(22, 20, 1, 20, Direction::East));
   ASSERT_TRUE(g.can_place_tile_and_figure(20, 20, 3, 1, Direction::SouthEast));
   ASSERT_TRUE(g.can_place_tile_and_figure(20, 21, 0, 5, Direction::EastNorth));
   ASSERT_FALSE(g.can_place_tile_and_figure(20, 18, 0, 8, Direction::North));
   ASSERT_FALSE(g.can_place_tile_and_figure(20, 17, 2, 13, Direction::East));
   ASSERT_FALSE(g.can_place_tile_and_figure(21, 18, 2, 7, Direction::West));
   ASSERT_TRUE(g.can_place_tile_and_figure(21, 18, 2, 7, Direction::North));
   ASSERT_FALSE(g.can_place_tile_and_figure(21, 18, 2, 7, Direction::SouthEast));
//   ASSERT_FALSE(g.can_place_tile_and_figure(20, 17, 0, 7, Direction::NorthWest));
}