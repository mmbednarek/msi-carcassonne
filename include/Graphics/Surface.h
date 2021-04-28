#ifndef MSI_CARCASSONNE_SURFACE_H
#define MSI_CARCASSONNE_SURFACE_H
#include "Context.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <cstdint>
#include <mb/int.h>
#include <mb/result.h>

namespace graphics {

struct Config {
   mb::i32 width;
   mb::i32 height;
   std::string title;
};

class Surface {
   SDL_Renderer *m_renderer = nullptr;
   SDL_Window *m_window = nullptr;

 public:
   constexpr Surface(SDL_Renderer *renderer, SDL_Window *window) : m_renderer(renderer), m_window(window) {}

   ~Surface();
   Surface(const Surface &) = delete;
   Surface &operator=(const Surface &) = delete;
   Surface(Surface &&other) noexcept;
   Surface &operator=(Surface &&other) noexcept;

   [[nodiscard]] constexpr Context context() const {
      return Context(m_renderer);
   }

   static mb::result<Surface> create(const Config &conf);

   [[nodiscard]] constexpr SDL_Renderer *raw() const {
      return m_renderer;
   }
};

}// namespace graphics

#endif//MSI_CARCASSONNE_SURFACE_H
