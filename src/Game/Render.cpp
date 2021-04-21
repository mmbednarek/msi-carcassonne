#include <MSI/Game/Game.h>
#include <MSI/Game/Render.h>
#include <mb/int.h>

namespace msi::game {

Render::Render(Game &game) : m_game(game) {}

void Render::render(const graphics::IContext &ctx) const noexcept {
   ctx.set_draw_color(0, 0, 0, 255);
   ctx.clear();

   constexpr auto w = 100;
   constexpr auto h = 100;

   auto &block_pos = m_game.block_pos();
   auto x = static_cast<mb::i32>(block_pos.x);
   auto y = static_cast<mb::i32>(block_pos.y);

   ctx.set_draw_color(255, 255, 0, 255);
   ctx.draw_line(x, y, x+w, y);
   ctx.set_draw_color(255, 0, 255, 255);
   ctx.draw_line(x+w, y, x+w, y+h);
   ctx.set_draw_color(0, 0, 255, 255);
   ctx.draw_line(x+w, y+h, x, y+h);
   ctx.set_draw_color(0, 255, 255, 255);
   ctx.draw_line(x, y+h, x, y);

   ctx.present();
}

}// namespace msi::game