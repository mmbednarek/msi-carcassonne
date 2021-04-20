#ifndef MSI_CARCASSONNE_SURFACE_H
#define MSI_CARCASSONNE_SURFACE_H
#include "Context.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <cstdint>

namespace msi::graphics {

struct Config {
   std::size_t width;
   std::size_t height;
};

template<typename T>
concept Renderable = requires(T t, const Context &ctx) {
   {t.render(ctx)};
};

class Surface {
   SDL_Renderer *m_renderer = nullptr;
   SDL_Window *m_window = nullptr;
   bool m_running = true;

 public:
   explicit Surface(const Config &conf);
   ~Surface();
   Surface(const Surface &) = delete;
   Surface &operator=(const Surface &) = delete;
   Surface(Surface &&other) noexcept;
   Surface &operator=(Surface &&other) noexcept;

   void render(const Renderable auto &r) {
      while (m_running) {
         r.render(Context(m_renderer));

         SDL_Event event;
         while (SDL_PollEvent(&event)) {
            on_event(event);
         }
      }
   }

 private:
   void on_event(SDL_Event &event) noexcept;
};

}// namespace msi::graphics

#endif//MSI_CARCASSONNE_SURFACE_H
