#ifndef MSI_CARCASSONNE_EVENTMANAGER_H
#define MSI_CARCASSONNE_EVENTMANAGER_H
#include <MSI/Input/Input.h>
#include <cstdint>

namespace msi::game {

class Game;

class EventManager {
   Game &m_game;
 public:
   explicit EventManager(Game &game);

   void on_quit();
   void on_keydown(input::KeyCode key);
   void on_keyup(input::KeyCode key);
   void on_button_down(std::uint8_t button);
   void on_button_up(std::uint8_t button);
   void on_mousemove(int x, int y);
};

}

#endif//MSI_CARCASSONNE_EVENTMANAGER_H
