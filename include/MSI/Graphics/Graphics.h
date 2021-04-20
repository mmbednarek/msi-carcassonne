/*
 * Graphics.h
 *
 * This is a wrapper library around SDL2 for graphics
 */
#ifndef MSI_CARCASSONNE_GRAPHICS_H
#define MSI_CARCASSONNE_GRAPHICS_H
#include "Surface.h"

namespace msi::graphics {

class Graphics {
 public:
   void render(const Context &ctx) const noexcept;
};

}

#endif //MSI_CARCASSONNE_GRAPHICS_H
