#ifndef MSI_CARCASSONNE_CONTEXT_H
#define MSI_CARCASSONNE_CONTEXT_H
#include "Texture.h"
#include <SDL2/SDL_render.h>

namespace graphics {

class Context {
   SDL_Renderer *m_renderer = nullptr;// reference
 public:
   explicit constexpr Context(SDL_Renderer *renderer) : m_renderer(renderer) {}

   void set_draw_color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a) const;
   void clear() const;
   void draw_line(int x1, int y1, int x2, int y2) const;
   void present() const;
   void draw(const Texture &tex, int x, int y, int w, int h) const;
   void draw(const Texture &tex, int tx, int ty, int tw, int th, int x, int y, int w, int h, double angle) const;
};

}// namespace graphics

#endif//MSI_CARCASSONNE_CONTEXT_H
