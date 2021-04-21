#ifndef MSI_CARCASSONNE_CONTEXT_H
#define MSI_CARCASSONNE_CONTEXT_H
#include "Texture.h"
#include <cstdint>

namespace msi::graphics {

class IContext {
 public:
   virtual void set_draw_color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a) const = 0;
   virtual void clear() const = 0;
   virtual void draw_line(int x1, int y1, int x2, int y2) const = 0;
   virtual void present() const = 0;
   virtual void draw(const Texture &tex, int x, int y, int w, int h) const = 0;
};

}// namespace msi::graphics

#endif//MSI_CARCASSONNE_CONTEXT_H
