#ifndef CARCASSONNE_GPU_HEURISTICPLAYER_H
#define CARCASSONNE_GPU_HEURISTICPLAYER_H
#ifdef __CUDACC__
#define DEVHOST __device__ __host__
#else
#define DEVHOST
#endif

#include <Carcassonne_GPU/Player.h>
#include <Carcassonne_GPU/Game/Game.h>
#include <Util_GPU/mb/result.h>

namespace carcassonne::ai {

class HeuristicPlayer {
   Player m_player;
 public:
   constexpr explicit HeuristicPlayer(Player player) noexcept : m_player(player) {}

   void await_turn(game::Game &game);

   game::FullMove make_move(game::Game &game, const Parameters &params = Parameters{}) noexcept;
};

}

#endif//CARCASSONNE_GPU_HEURISTICPLAYER_H
