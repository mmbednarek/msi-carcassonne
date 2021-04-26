#include <Carcassonne/Game/Game.h>

namespace carcassonne::game {

void Game::update() {
}

const Position &Game::block_pos() const noexcept {
   return m_block_pos;
}

void Game::set_block_pos(mb::f64 x, mb::f64 y) noexcept {
   m_block_pos.x = x;
   m_block_pos.y = y;
}
IBoard &Game::board() noexcept {
   return m_board;
}

}