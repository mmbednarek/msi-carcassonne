#include <Carcassonne/IGame.h>
#include <Carcassonne/Frontend/Object.h>
#include <Carcassonne/Frontend/Resource.h>

namespace carcassonne::frontend {

constexpr auto g_tile_size = 96;
constexpr auto g_figure_size = 96;

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

void render_figure(const graphics::Context &ctx, const Camera &cam, double x, double y, Player p) {
   ctx.draw(ResourceManager::texture(TextureResource::Figures),
            g_figure_size * static_cast<int>(p),
            0,
            g_figure_size,
            g_figure_size,
            static_cast<int>(cam.translate_x(x - 0.25)),
            static_cast<int>(cam.translate_y(y - 0.25)),
            static_cast<int>(cam.scale(0.5)),
            static_cast<int>(cam.scale(0.5)),
            0.0);
}

}// namespace carcassonne::frontend