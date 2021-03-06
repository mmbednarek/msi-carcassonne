#ifndef MSI_CARCASSONNE_TILEVIEW_H
#define MSI_CARCASSONNE_TILEVIEW_H
#include "Camera.h"
#include <Graphics/Context.h>

namespace carcassonne::frontend {

void render_tile(const graphics::Context &ctx, const Camera &cam, int x, int y, TileType type, mb::u8 rotation);
void render_figure(const graphics::Context &ctx, const Camera &cam, double x, double y, Player p);

}

#endif//MSI_CARCASSONNE_TILEVIEW_H
