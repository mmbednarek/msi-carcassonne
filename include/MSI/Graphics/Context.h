#ifndef MSI_CARCASSONNE_CONTEXT_H
#define MSI_CARCASSONNE_CONTEXT_H
#include <SDL2/SDL_render.h>

namespace msi::graphics {

class Context {
   SDL_Renderer *m_renderer = nullptr;// reference
 public:
   explicit Context(SDL_Renderer *renderer);

   void set_draw_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) const;
   void clear() const;
   void draw_line(int x1, int y1, int x2, int y2) const;
   void present() const;
};

}

#endif//MSI_CARCASSONNE_CONTEXT_H
