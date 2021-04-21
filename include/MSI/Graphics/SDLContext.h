#ifndef MSI_CARCASSONNE_SDLCONTEXT_H
#define MSI_CARCASSONNE_SDLCONTEXT_H
#include "Context.h"
#include <SDL2/SDL_render.h>

namespace msi::graphics {

class SDLContext : public IContext {
   SDL_Renderer *m_renderer = nullptr;// reference
 public:
   explicit constexpr SDLContext(SDL_Renderer *renderer) : m_renderer(renderer) {}

   void set_draw_color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a) const override;
   void clear() const override;
   void draw_line(int x1, int y1, int x2, int y2) const override;
   void present() const override;
};

}// namespace msi::graphics

#endif//MSI_CARCASSONNE_SDLCONTEXT_H
