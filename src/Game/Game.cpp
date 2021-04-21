#include <MSI/Game/Game.h>

namespace msi::game {

Game::Game() : m_event_manager(*this), m_render(*this) {}

void Game::update() {
}

}