#ifndef MSI_CARCASSONNE_MOVE_H
#define MSI_CARCASSONNE_MOVE_H
#include <Carcassonne/Core.h>

namespace carcassonne::game {

class Game;

class Move : public IMove {
   Game &m_game;
   Player m_player;
   TileType m_tile_type;
   MovePhase m_phase = MovePhase::PlaceTile;
   int m_x{}, m_y{};

 public:
   constexpr explicit Move(Player p, TileType t, Game &game) : m_game(game), m_player(p), m_tile_type(t) {}

   [[nodiscard]] Player player() const noexcept override;
   [[nodiscard]] TileType tile_type() const noexcept override;
   [[nodiscard]] MovePhase phase() const noexcept override;
   [[nodiscard]] TilePosition position() const noexcept override;
   [[nodiscard]] bool is_free(Direction d) const noexcept override;
   void place_tile(int x, int y, mb::u8 rotation) noexcept override;
   void place_figure(Direction d) noexcept override;
   void ignore_figure() noexcept override;
};

}// namespace carcassonne::game

#endif//MSI_CARCASSONNE_MOVE_H
