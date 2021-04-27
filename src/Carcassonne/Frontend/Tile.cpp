#include <Carcassonne/Frontend/Resource.h>
#include <Carcassonne/Frontend/Tile.h>

namespace carcassonne::frontend {

constexpr auto g_tile_size = 96;

void render_tile(const graphics::Context &ctx, const Camera &cam, int x, int y, TileType type, mb::u8 rotation) {
   const auto palette_x = (type - 1) % 4;
   const auto palette_y = (type - 1) / 4;
   ctx.draw(ResourceManager::texture(TextureResource::Tiles),
            g_tile_size * palette_x,
            g_tile_size * palette_y,
            g_tile_size,
            g_tile_size,
            static_cast<int>(cam.translate_x(x)),
            static_cast<int>(cam.translate_y(y)),
            static_cast<int>(cam.scale(1.0)),
            static_cast<int>(cam.scale(1.0)),
            90.0 * rotation);
}

}// namespace carcassonne::frontend