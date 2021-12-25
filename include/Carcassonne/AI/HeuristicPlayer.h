#ifndef MSI_CARCASSONNE_HEURISTICPLAYER_H
#define MSI_CARCASSONNE_HEURISTICPLAYER_H
#include <Carcassonne/Player.h>
#include <Carcassonne/IGame.h>
#include <mb/result.h>

namespace carcassonne::ai {

class HeuristicPlayer {
   Player m_player;
 public:
   constexpr explicit HeuristicPlayer(Player player) noexcept : m_player(player) {}

   void await_turn(std::unique_ptr<IGame> &game);

   mb::result<FullMove> make_move(std::unique_ptr<IGame> &game, const Parameters &params = Parameters{}) noexcept;
};

}

#endif//MSI_CARCASSONNE_HEURISTICPLAYER_H
