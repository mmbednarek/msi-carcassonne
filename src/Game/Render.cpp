#include <Carcassonne/Game/Game.h>
#include <Carcassonne/Game/Render.h>
#include <Carcassonne/Graphics/Resource.h>
#include <mb/int.h>

namespace carcassonne::game {

using graphics::TextureResource;

Render::Render(Game &game) : m_game(game) {}

void Render::render(const graphics::IContext &ctx) const noexcept {
   ctx.set_draw_color(81, 170, 162, 255);
   ctx.clear();

   constexpr auto w = 96;
   constexpr auto h = 96;

   auto &block_pos = m_game.block_pos();
   auto x = static_cast<mb::i32>(block_pos.x);
   auto y = static_cast<mb::i32>(block_pos.y);

   ctx.set_draw_color(214, 203, 108, 255);
   ctx.draw_line(x, y, x + w, y);
   ctx.draw_line(x + w, y, x + w, y + h);
   ctx.draw_line(x + w, y + h, x, y + h);
   ctx.draw_line(x, y + h, x, y);

   ctx.draw(graphics::ResourceManager::texture(TextureResource::YellowFigure), x, y, w, h);

   ctx.present();
}

}// namespace carcassonne::game