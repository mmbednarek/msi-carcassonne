#ifndef MSI_CARCASSONNE_RANDOM_PLAYER_H
#define MSI_CARCASSONNE_RANDOM_PLAYER_H
#include <Carcassonne/Core.h>

// AI implementation

namespace carcassonne::game::ai {

class RandomPlayer {
   IGame &m_game;
   
 public:
   explicit RandomPlayer(IGame &game);
};

} // namespace carcassonne::game::ai

#endif //MSI_CARCASSONNE_RANDOM_PLAYER_H
