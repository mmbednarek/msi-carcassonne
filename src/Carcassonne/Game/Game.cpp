#include <Carcassonne/Game/Game.h>
#include <Carcassonne/Game/Move.h>
#include <algorithm>

namespace carcassonne::game {

// Game::Game() : m_random_generator(m_random_device()) {}
<<<<<<< HEAD
Game::Game() : m_random_generator(6) {}
=======
Game::Game() : m_random_generator(3) {}
>>>>>>> added connection middle-N/E/S/W

const IBoard &Game::board() const noexcept {
   return m_board;
}

Player Game::current_player() const noexcept {
   return m_current_player;
}

std::unique_ptr<IMove> Game::new_move(Player p) noexcept {
   return std::make_unique<Move>(p, (m_random_distribution(m_random_generator) % 24) + 1, *this);
}

mb::view<Figure> Game::figures() const noexcept {
   return m_figures;
}

void Game::add_figure(Figure f) {
   m_figures.push_back(f);
}

}// namespace carcassonne::game