#include <Tournament/Tournament.h>

namespace tournament {
   using carcassonne::Player;

Tournament_stats create_tournament (std::vector<carcassonne::game::Game> games, AI_Player player_type_A, AI_Player player_type_B) {
   Tournament_stats stats;
   std::random_device random_device;
   std::mt19937 random_generator(random_device());

   if (player_type_A == AI_Player::Random && player_type_B == AI_Player::Random) {
      auto player_A = carcassonne::ai::RandomPlayer<>(random_generator, Player::Black);
      auto player_B = carcassonne::ai::RandomPlayer<>(random_generator, Player::Black);
   }
   if (player_type_A == AI_Player::Random && player_type_B == AI_Player::Heuristic) {
      auto player_A = carcassonne::ai::RandomPlayer<>(random_generator, Player::Black);
      auto player_B = carcassonne::ai::HeuristicPlayer(Player::Black);
   }
   if (player_type_A == AI_Player::Random && player_type_B == AI_Player::Mcts_random) {
      auto player_A = carcassonne::ai::RandomPlayer<>(random_generator, Player::Black);
      auto player_B = carcassonne::ai::RandomPlayer<>(random_generator, Player::Blue);
   }
   if (player_type_A == AI_Player::Random && player_type_B == AI_Player::Mcts_heuristic) {
      auto player_A = carcassonne::ai::RandomPlayer<>(random_generator, Player::Black);
      auto player_B = carcassonne::ai::RandomPlayer<>(random_generator, Player::Blue);
   }
   if (player_type_A == AI_Player::Heuristic && player_type_B == AI_Player::Heuristic) {
      auto player_A = carcassonne::ai::RandomPlayer<>(random_generator, Player::Black);
      auto player_B = carcassonne::ai::RandomPlayer<>(random_generator, Player::Blue);
   }
   if (player_type_A == AI_Player::Heuristic && player_type_B == AI_Player::Mcts_random) {
      auto player_A = carcassonne::ai::RandomPlayer<>(random_generator, Player::Black);
      auto player_B = carcassonne::ai::RandomPlayer<>(random_generator, Player::Blue);
   }
   if (player_type_A == AI_Player::Heuristic && player_type_B == AI_Player::Mcts_heuristic) {
      auto player_A = carcassonne::ai::RandomPlayer<>(random_generator, Player::Black);
      auto player_B = carcassonne::ai::RandomPlayer<>(random_generator, Player::Blue);
   }
   if (player_type_A == AI_Player::Mcts_random && player_type_B == AI_Player::Mcts_random) {
      auto player_A = carcassonne::ai::RandomPlayer<>(random_generator, Player::Black);
      auto player_B = carcassonne::ai::RandomPlayer<>(random_generator, Player::Blue);
   }
   if (player_type_A == AI_Player::Mcts_random && player_type_B == AI_Player::Mcts_heuristic) {
      auto player_A = carcassonne::ai::RandomPlayer<>(random_generator, Player::Black);
      auto player_B = carcassonne::ai::RandomPlayer<>(random_generator, Player::Blue);
   }
   if (player_type_A == AI_Player::Mcts_heuristic && player_type_B == AI_Player::Mcts_heuristic) {
      auto player_A = carcassonne::ai::RandomPlayer<>(random_generator, Player::Black);
      auto player_B = carcassonne::ai::RandomPlayer<>(random_generator, Player::Blue);
   }


   // std::vector<carcassonne::ai::RandomPlayer<>> random_players;
   // random_players.reserve(random_ai_player_count);
   // std::transform(carcassonne::g_players.begin(), carcassonne::g_players.begin() + (human_player_count + random_ai_player_count), std::back_inserter(random_players), [&random_generator](carcassonne::Player p) {
   //    return carcassonne::ai::RandomPlayer<>(random_generator, p);
   // });
   // for (auto &player : random_players) {
   //    player.await_turn(game);
   // }

   // std::vector<carcassonne::ai::HeuristicPlayer> heuristic_players;
   // heuristic_players.reserve(heuristic_ai_player_count);
   // std::transform(carcassonne::g_players.begin() + (human_player_count + random_ai_player_count), carcassonne::g_players.begin() + (human_player_count + random_ai_player_count + heuristic_ai_player_count), std::back_inserter(heuristic_players), [](carcassonne::Player p) {
   //    return carcassonne::ai::HeuristicPlayer(p);
   // });
   // for (auto &player : heuristic_players) {
   //    player.await_turn(game);
   // }

   // std::vector<std::unique_ptr<carcassonne::ai::MCTSPlayer>> mcts_players(mcts_ai_player_count);
   // std::transform(carcassonne::g_players.begin() + (human_player_count + random_ai_player_count + heuristic_ai_player_count), carcassonne::g_players.begin() + (human_player_count + random_ai_player_count + heuristic_ai_player_count + mcts_ai_player_count), mcts_players.begin(), [&game](carcassonne::Player p) {
   //    return std::make_unique<carcassonne::ai::MCTSPlayer>(game, p);
   // });

   // constexpr double dt = 1000.0 / 60.0;
   // auto prev_time = static_cast<double>(now_milis());
   // auto dt_accum = 0.0;

   // game.start();

   // while (view.status() != Status::Quitting) {
   //    auto now = static_cast<double>(now_milis());
   //    auto diff = now - prev_time;
   //    prev_time = now;

   //    dt_accum += diff;
   //    while (dt_accum > dt) {
   //       dt_accum -= dt;
   //       game.update(dt);
   //    }
   // }
   

   return stats;
}

} //namespace tournament 