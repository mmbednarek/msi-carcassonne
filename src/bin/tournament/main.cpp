#include <Tournament/Tournament.h>

uint64_t now_milis() {
   return std::chrono::duration_cast<std::chrono::milliseconds>(
                  std::chrono::system_clock::now().time_since_epoch())
           .count();
}

constexpr bool player_range_ok(int count) {
   return count >= 0 && count <= 4;
}

int main() {

   auto game_seed = std::stoull(mb::getenv("C_SEED").unwrap("9"));
   auto human_player_count = std::stoi(mb::getenv("C_HUMAN_PLAYERS").unwrap("1"));
   auto random_ai_player_count = std::stoi(mb::getenv("C_AI_RANDOM_PLAYERS").unwrap("0"));
   auto mcts_ai_player_count = std::stoi(mb::getenv("C_AI_MCTS_PLAYERS").unwrap("1"));
   auto heuristic_ai_player_count = std::stoi(mb::getenv("C_AI_HEURISTIC_PLAYERS").unwrap("0"));

   if (!player_range_ok(human_player_count)) {
      fmt::print(stderr, "invalid human player count (0-4): {}", human_player_count);
      return 1;
   }

   if (!player_range_ok(random_ai_player_count)) {
      fmt::print(stderr, "invalid random ai player count (0-4): {}", random_ai_player_count);
      return 1;
   }

   if (!player_range_ok(mcts_ai_player_count)) {
      fmt::print(stderr, "invalid random ai player count (0-4): {}", random_ai_player_count);
      return 1;
   }

   if (!player_range_ok(human_player_count + random_ai_player_count + mcts_ai_player_count + heuristic_ai_player_count)) {
      fmt::print(stderr, "invalid player count (0-4): {}", human_player_count + random_ai_player_count + mcts_ai_player_count + heuristic_ai_player_count);
      return 1;
   }

   std::vector<carcassonne::game::Game> random_vs_random_games,
                                        random_vs_heuristic_games,
                                        random_vs_mcts_random_games,
                                        random_vs_mcts_heuristic_games,
                                        heuristic_vs_heuristic_games,
                                        heuristic_vs_mcts_random_games,
                                        heuristic_vs_mcts_heuristic_games,
                                        mcts_random_vs_mcts_random_games,
                                        mcts_random_vs_mcts_heuristic_games,
                                        mcts_heuristic_vs_mcts_heuristic_games;

   return 0;
}
