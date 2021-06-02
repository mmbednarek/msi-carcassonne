#ifndef MSI_CARCASSONNE_RANDOM_PLAYER_H
#define MSI_CARCASSONNE_RANDOM_PLAYER_H
#include <Carcassonne/IGame.h>
#include <Util/Random.h>
#include <random>

// AI implementation

namespace carcassonne::ai {

template<typename Generator = std::mt19937>
class RandomPlayer {
   Player m_player;

   Generator &m_random_generator;
 public:
   explicit RandomPlayer(Generator &generator, Player player) : m_random_generator(generator),
                                                                m_player(player) {}

   void await_turn(IGame &game) {
      game.on_next_move([this](IGame &game, Player player) {
         if (player != m_player)
            return;
         make_move(game);
      });
   }

   void make_move(IGame &game) noexcept {
      auto move = game.new_move(m_player);

      const auto possible_tile_moves = game.moves(move->tile_type());
      auto tile_placement_it = util::random_from_range(m_random_generator, possible_tile_moves.begin(), possible_tile_moves.end() - 1);
      const auto tile_placement = *tile_placement_it;

      move->place_tile_at(tile_placement.x, tile_placement.y, tile_placement.rotation);
      if (move->phase() == MovePhase::Done) {
         return;
      }

      const auto possible_figure_moves = game.figure_placements(tile_placement.x, tile_placement.y);
      if (!possible_figure_moves.empty()) {
         const auto direction_it = util::random_from_range(m_random_generator, possible_figure_moves.cbegin(), possible_figure_moves.cend());
         if (direction_it == possible_figure_moves.cend()) {
            move->ignore_figure();
         } else {
            move->place_figure(*direction_it);
         }
      } else {
         move->ignore_figure();
      }
   }
};

}// namespace carcassonne::ai

#endif//MSI_CARCASSONNE_RANDOM_PLAYER_H
