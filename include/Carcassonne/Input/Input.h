#ifndef MSI_CARCASSONNE_INPUT_H
#define MSI_CARCASSONNE_INPUT_H
#include <SDL2/SDL.h>

namespace carcassonne::input {

using KeyCode = int;

template<typename T>
concept EventManager = requires(T t) {
   {t.on_quit()};
   {t.on_keydown(static_cast<KeyCode>(0))};
   {t.on_keyup(static_cast<KeyCode>(0))};
   {t.on_button_down(static_cast<uint>(0))};
   {t.on_button_up(static_cast<uint>(0))};
   {t.on_mousemove(0, 0)};
};

void handle_events(EventManager auto &em) {
   SDL_Event event;
   while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
         em.on_quit();
         break;
      case SDL_KEYDOWN:
         em.on_keydown(event.key.keysym.sym);
         break;
      case SDL_KEYUP:
         em.on_keyup(event.key.keysym.sym);
         break;
      case SDL_MOUSEBUTTONDOWN:
         em.on_button_down(event.button.button);
         break;
      case SDL_MOUSEBUTTONUP:
         em.on_button_up(event.button.button);
         break;
      case SDL_MOUSEMOTION:
         em.on_mousemove(event.motion.x, event.motion.y);
         break;
      }
   }
}

}

#endif//MSI_CARCASSONNE_INPUT_H
