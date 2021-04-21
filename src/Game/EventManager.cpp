#include <MSI/Game/EventManager.h>
#include <MSI/Game/Game.h>

namespace msi::game {

EventManager::EventManager(Game &game) : m_game(game) {}

void EventManager::on_quit() {
   m_game.set_status(Status::Quitting);
}

void EventManager::on_keydown(input::KeyCode key) {
   constexpr auto speed = 10.0f;

   switch (key) {
   case SDLK_w:
      m_game.block_pos().y -= speed;
      break;
   case SDLK_a:
      m_game.block_pos().x -= speed;
      break;
   case SDLK_s:
      m_game.block_pos().y += speed;
      break;
   case SDLK_d:
      m_game.block_pos().x += speed;
      break;
   default: break;
   }
}

void EventManager::on_keyup(input::KeyCode key) {
}

void EventManager::on_button_down(std::uint8_t button) {
}

void EventManager::on_button_up(std::uint8_t button) {
}

void EventManager::on_mousemove(int x, int y) {
}

}// namespace msi::game