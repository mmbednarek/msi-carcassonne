#include <Carcassonne/Game/Game_GPU.h>
#include <Carcassonne/Game/Move_GPU.h>
#include <Carcassonne/IMove.h>
#include <memory>
#include <fmt/core.h>

namespace carcassonne::game {

Player Move_GPU::player() const noexcept {
   return m_player;
}

TileType Move_GPU::tile_type() const noexcept {
   return m_tile_type;
}

mb::result<mb::empty> Move_GPU::place_tile_at(int x, int y, mb::u8 rotation) noexcept {
   return mb::ok;
}

mb::result<mb::empty> Move_GPU::place_figure(Direction d) noexcept {
   return mb::ok;
}

mb::result<mb::empty> Move_GPU::ignore_figure() noexcept {
   return mb::ok;
}

MovePhase Move_GPU::phase() const noexcept {
   return m_phase;
}

TilePosition Move_GPU::position() const noexcept {
   return TilePosition{m_x, m_y};
}

bool Move_GPU::is_free(Direction d) const noexcept {
   return m_game.can_place_figure(m_x, m_y, d);
}

std::unique_ptr<IMove> Move_GPU::clone(std::unique_ptr<IGame> &game) const noexcept {
   return std::make_unique<Move_GPU>(*dynamic_cast<Game_GPU *>(game.get()), m_player, m_tile_type, m_x, m_y);
}

mb::result<mb::empty> Move_GPU::place_tile(TileMove tile_location) noexcept {
   return place_tile_at(tile_location.x, tile_location.y, tile_location.rotation);
}

}
// namespace carcassonne::game
