#include <Carcassonne/Game/Game.h>

namespace carcassonne::game {

Game::Game() : m_event_manager(*this), m_render(*this) {}

void Game::update() {
}

}