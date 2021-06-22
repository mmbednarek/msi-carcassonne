#ifndef MSI_TOURNAMENT_H
#define MSI_TOURNAMENT_H

#include <Carcassonne/AI/HeuristicPlayer.h>
#include <Carcassonne/AI/MCTSPlayer.h>
#include <Carcassonne/AI/RandomPlayer.h>
#include <Carcassonne/Game/Game.h>
#include <fmt/core.h>
#include <mb/core.h>


namespace tournament {

struct Tournament_stats {
   mb::i16 wins_count = 0;
   mb::i16 loses_count = 0;
   mb::i8 winner_points = 0;
   mb::i8 losser_points = 0;
   carcassonne::Player winner;
};

enum class AI_Player : mb::u8 {
   Random = 0,
   Heuristic = 1,
   Mcts_random = 2,
   Mcts_heuristic = 3,
};

constexpr std::array<AI_Player, 4> g_players{
        AI_Player::Random,
        AI_Player::Heuristic,
        AI_Player::Mcts_random,
        AI_Player::Mcts_heuristic,
};


Tournament_stats create_tournament (std::vector<carcassonne::game::Game> games, AI_Player player_A, AI_Player player_B);

} //namespace tournament 

#endif // MSI_TOURNAMENT_H