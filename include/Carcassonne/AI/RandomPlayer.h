#ifndef MSI_CARCASSONNE_RANDOM_PLAYER_H
#define MSI_CARCASSONNE_RANDOM_PLAYER_H
#include <Carcassonne/Core.h>
#include <random>

// AI implementation

namespace carcassonne::ai {

class RandomPlayer {
   Player m_player;

   std::random_device m_random_device;
   std::mt19937_64 m_random_generator;
 public:
   explicit RandomPlayer(IGame &game, Player player);

   void make_move(IGame &game) noexcept;
};

} // namespace carcassonne::game::ai

#endif //MSI_CARCASSONNE_RANDOM_PLAYER_H
