#include <Carcassonne/Groups.h>
#include <gtest/gtest.h>

TEST(Util, Groups) {
   {
      carcassonne::Groups<4> g;
      for (mb::size i = 0; i < 4; ++i) {
         EXPECT_EQ(g.group_of(i), i);
      }

      g.assign(2, carcassonne::Player::Yellow);
      EXPECT_TRUE(g.is_free(0));

      g.set_type(0, carcassonne::EdgeType::Town);

      g.join(0, 1);
      g.join(2, 3);
      g.join(1, 2);

      EXPECT_FALSE(g.is_free(2));
      EXPECT_FALSE(g.is_free(0));

      for (mb::size i = 0; i < 4; ++i) {
         EXPECT_EQ(g.group_of(i), 0);
         EXPECT_EQ(g.type_of(i), carcassonne::EdgeType::Town);
      }
   }
   {
      carcassonne::Groups<1024> g;
      for (mb::size i = 0; i < 1024; ++i) {
         EXPECT_EQ(g.group_of(i), i);
      }

      g.assign(5, carcassonne::Player::Red);

      auto last = 0;
      auto current = 1;
      while (current < 1024) {
         g.join(last, current);
         auto tmp = current;
         current += last;
         last = tmp;
      }

      EXPECT_FALSE(g.is_free(0));
      EXPECT_TRUE(g.is_free(10));

      last = 0;
      current = 1;
      EXPECT_EQ(g.group_of(0), 0);
      while (current < 1024) {
         EXPECT_EQ(g.group_of(current), 0);
         for (auto i = last + 1; i < current; ++i) {
            EXPECT_NE(g.group_of(i), 0);
         }
         auto tmp = current;
         current += last;
         last = tmp;
      }
   }
}