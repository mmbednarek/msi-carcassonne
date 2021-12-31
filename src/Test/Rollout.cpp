#include <Carcassonne/Encoder/Rollout.h>
#include <Carcassonne/Game/Game.h>
#include <Carcassonne/Move.h>
#include <gtest/gtest.h>

TEST(Rollout, encode_decode) {
   carcassonne::encoder::Rollout rollout(2, 2137);

   rollout.register_move(carcassonne::FullMove{
           .x = 20,
           .y = 21,
           .rotation = 0,
           .ignored_figure = false,
           .direction = carcassonne::Direction::Middle,
   });

   rollout.register_move(carcassonne::FullMove{
           .x = 20,
           .y = 19,
           .rotation = 2,
           .ignored_figure = true,
   });

   auto game = rollout.as_game();
   ASSERT_NE(game.get(), nullptr);

   std::stringstream ss;
   rollout.encode(ss);

   carcassonne::encoder::Rollout decoded_rollout(0, 0);
   decoded_rollout.decode(ss);

   auto decoded_game = rollout.as_game();
   ASSERT_NE(decoded_game.get(), nullptr);
}