#ifndef MSI_CARCASSONNE_DEEPRL_PLAYER_H
#define MSI_CARCASSONNE_DEEPRL_PLAYER_H
#include "Network.h"
#include <Carcassonne/IGame.h>
#include <Carcassonne/AI/DeepRL.h>
#include <random>

namespace carcassonne::ai {

enum class SimulationType {
   Heuristic,
   Random
};

class DeepRLPlayer {
   Player m_player;
   std::array<FullMove, 4> m_last_moves{};
   Network m_network;
   mb::size m_player_count;
   SimulationType m_simulation_type{};
 public:
   explicit DeepRLPlayer(IGame &game, Player player, SimulationType sim_type);

   void train_network(const IGame &game);
   void make_move(IGame &game) noexcept;
};

} // namespace carcassonne::game::ai

#endif //MSI_CARCASSONNE_DEEPRL_PLAYER_H
