#include <Tournament/Tournament.h>

namespace tournament {
   using carcassonne::Player;
   using carcassonne::ai::RandomPlayer;
   using carcassonne::ai::HeuristicPlayer;
   using carcassonne::ai::MCTSPlayer;
   using carcassonne::ai::SimulationType;
   using carcassonne::game::Game;
   using Games = std::vector<Game>;

Tournament_stats create_tournament (Games games, AI_Player player_type_A, AI_Player player_type_B) {
   Tournament_stats stats;
   std::random_device random_device;
   std::mt19937 random_generator(random_device());

   for (auto game : games) {
      if (player_type_A == AI_Player::Random && player_type_B == AI_Player::Random) {
         auto player_A = RandomPlayer<>(random_generator, Player::Black);
         auto player_B = RandomPlayer<>(random_generator, Player::Blue);
         player_A.await_turn(game);
         player_B.await_turn(game);
      }
      if (player_type_A == AI_Player::Random && player_type_B == AI_Player::Heuristic) {
         auto player_A = RandomPlayer<>(random_generator, Player::Black);
         auto player_B = HeuristicPlayer(Player::Blue);
         player_A.await_turn(game);
         player_B.await_turn(game);
      }
      if (player_type_A == AI_Player::Random && player_type_B == AI_Player::Mcts_random) {
         auto player_A = RandomPlayer<>(random_generator, Player::Black);
         auto player_B = std::make_unique<MCTSPlayer>(game, Player::Blue, SimulationType::Random);
         player_A.await_turn(game);
      }
      if (player_type_A == AI_Player::Random && player_type_B == AI_Player::Mcts_heuristic) {
         auto player_A = RandomPlayer<>(random_generator, Player::Black);
         auto player_B = std::make_unique<MCTSPlayer>(game, Player::Blue, SimulationType::Heuristic);
         player_A.await_turn(game);
      }


      if (player_type_A == AI_Player::Heuristic && player_type_B == AI_Player::Random) {
         auto player_A = HeuristicPlayer(Player::Black);
         auto player_B = RandomPlayer<>(random_generator, Player::Blue);
         player_A.await_turn(game);
         player_B.await_turn(game);
      }
      if (player_type_A == AI_Player::Heuristic && player_type_B == AI_Player::Heuristic) {
         auto player_A = HeuristicPlayer(Player::Black);
         auto player_B = HeuristicPlayer(Player::Blue);
         player_A.await_turn(game);
         player_B.await_turn(game);
      }
      if (player_type_A == AI_Player::Heuristic && player_type_B == AI_Player::Mcts_random) {
         auto player_A = HeuristicPlayer(Player::Black);
         auto player_B = std::make_unique<MCTSPlayer>(game, Player::Blue, SimulationType::Random);
         player_A.await_turn(game);
      }
      if (player_type_A == AI_Player::Heuristic && player_type_B == AI_Player::Mcts_heuristic) {
         auto player_A = HeuristicPlayer(Player::Black);
         auto player_B = std::make_unique<MCTSPlayer>(game, Player::Blue, SimulationType::Heuristic);
         player_A.await_turn(game);
      }

      if (player_type_A == AI_Player::Mcts_random && player_type_B == AI_Player::Random) {
         auto player_A = std::make_unique<MCTSPlayer>(game, Player::Black, SimulationType::Random);
         auto player_B = RandomPlayer<>(random_generator, Player::Blue);
         player_B.await_turn(game);
      }
      if (player_type_A == AI_Player::Mcts_random && player_type_B == AI_Player::Heuristic) {
         auto player_A = std::make_unique<MCTSPlayer>(game, Player::Black, SimulationType::Random);
         auto player_B = HeuristicPlayer(Player::Blue);
         player_B.await_turn(game);
      }
      if (player_type_A == AI_Player::Mcts_random && player_type_B == AI_Player::Mcts_random) {
         auto player_A = std::make_unique<MCTSPlayer>(game, Player::Black, SimulationType::Random);
         auto player_B = std::make_unique<MCTSPlayer>(game, Player::Blue, SimulationType::Random);
      }
      if (player_type_A == AI_Player::Mcts_random && player_type_B == AI_Player::Mcts_heuristic) {
         auto player_A = std::make_unique<MCTSPlayer>(game, Player::Black, SimulationType::Random);
         auto player_B = std::make_unique<MCTSPlayer>(game, Player::Blue, SimulationType::Heuristic);
      }

      if (player_type_B == AI_Player::Random && player_type_A == AI_Player::Mcts_heuristic) {
         auto player_B = RandomPlayer<>(random_generator, Player::Blue);
         auto player_A = std::make_unique<MCTSPlayer>(game, Player::Black, SimulationType::Heuristic);
         player_B.await_turn(game);
      }
      if (player_type_B == AI_Player::Heuristic && player_type_A == AI_Player::Mcts_heuristic) {
         auto player_B = HeuristicPlayer(Player::Blue);
         auto player_A = std::make_unique<MCTSPlayer>(game, Player::Black, SimulationType::Heuristic);
         player_B.await_turn(game);
      }
      if (player_type_A == AI_Player::Mcts_heuristic && player_type_B == AI_Player::Mcts_random) {
         auto player_A = std::make_unique<MCTSPlayer>(game, Player::Black, SimulationType::Heuristic);
         auto player_B = std::make_unique<MCTSPlayer>(game, Player::Blue, SimulationType::Random);
      }
      if (player_type_A == AI_Player::Mcts_heuristic && player_type_B == AI_Player::Mcts_heuristic) {
         auto player_A = std::make_unique<MCTSPlayer>(game, Player::Black, SimulationType::Heuristic);
         auto player_B = std::make_unique<MCTSPlayer>(game, Player::Blue, SimulationType::Heuristic);
      }


      constexpr double dt = 1000.0 / 60.0;
      game.update(dt);
   }

   return stats;
}

} //namespace tournament 