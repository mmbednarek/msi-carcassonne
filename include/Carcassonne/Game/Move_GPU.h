#ifndef MSI_CARCASSONNE_GAME_MOVE_GPU_H
#define MSI_CARCASSONNE_GAME_MOVE_GPU_H
#include <Carcassonne/IGame.h>

namespace carcassonne::game {

class Game_GPU;

class Move_GPU : public IMove {
   Game_GPU &m_game;
   Player m_player;
   TileType m_tile_type;
   MovePhase m_phase = MovePhase::PlaceTile;
   int m_x{}, m_y{};
   mb::u8 m_rotation{};

 public:
   constexpr Move_GPU(Player p, TileType t, Game_GPU &game) : m_game(game), m_player(p), m_tile_type(t) {}

   constexpr Move_GPU(Game_GPU &game, Player player, TileType tile_type, int x, int y) : m_game(game),
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
   std::unique_ptr<IMove> clone(std::unique_ptr<IGame> &game) const noexcept override;
};

}// namespace carcassonne::game

#endif//MSI_CARCASSONNE_GAME_MOVE_GPU_H
