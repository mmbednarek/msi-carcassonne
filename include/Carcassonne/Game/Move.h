#ifndef MSI_CARCASSONNE_GAME_MOVE_H
#define MSI_CARCASSONNE_GAME_MOVE_H
#include <Carcassonne/IGame.h>

namespace carcassonne::game {

class Game;

class Move : public IMove {
   Game &m_game;
   Player m_player;
   TileType m_tile_type;
   int m_x{};
   int m_y{};
   MovePhase m_phase = MovePhase::PlaceTile;
   mb::u8 m_rotation{};

 public:
   constexpr Move(Player p, TileType t, Game &game) : m_game(game), m_player(p), m_tile_type(t) {}

   constexpr Move(Game &game, Player player, TileType tile_type, int x, int y) : m_game(game),
                                                                                 m_player(player),
                                                                                 m_tile_type(tile_type),
                                                                                 m_x(x),
                                                                                 m_y(y),
                                                                                 m_phase(MovePhase::PlaceFigure) {}

   [[nodiscard]] Player player() const noexcept override;
   [[nodiscard]] TileType tile_type() const noexcept override;
   [[nodiscard]] MovePhase phase() const noexcept override;
   [[nodiscard]] TilePosition position() const noexcept override;
   [[nodiscard]] bool is_free(Direction d) const noexcept override;
   mb::result<mb::empty> place_tile(TileMove tile_location) noexcept override;
   mb::result<mb::empty> place_tile_at(int x, int y, mb::u8 rotation) noexcept override;
   mb::result<mb::empty> place_figure(Direction d) noexcept override;
   mb::result<mb::empty> ignore_figure() noexcept override;
   std::unique_ptr<IMove> clone(IGame &game) const noexcept override;
};

}// namespace carcassonne::game

#endif//MSI_CARCASSONNE_GAME_MOVE_H
