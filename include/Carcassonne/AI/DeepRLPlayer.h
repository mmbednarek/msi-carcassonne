#ifndef MSI_CARCASSONNE_DEEPRL_PLAYER_H
#define MSI_CARCASSONNE_DEEPRL_PLAYER_H
#include <Carcassonne/AI/DeepRL.h>
#include <Carcassonne/AI/Tree.h>
#include <Carcassonne/IGame.h>
#include <Carcassonne/RL/Network.h>
#include <Carcassonne/RL/Concurrency.h>
#include <random>
#include <thread>

namespace carcassonne::ai {

class DeepRLPlayer {
   Player m_player;
   std::array<FullMove, 4> m_last_moves{};
   mb::size m_player_count;
   std::unique_ptr<rl::client_threads> m_clients_pool = nullptr;
   std::unique_ptr<Tree> prepare_tree(const IGame &game);

 public:
   explicit DeepRLPlayer(
      IGame &game,
      Player player);
   void train_network(const IGame &game); // to be implemented
   void make_move(IGame &game) noexcept;
};

}// namespace carcassonne::ai

#endif//MSI_CARCASSONNE_DEEPRL_PLAYER_H
