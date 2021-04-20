#include <MSI/Graphics/Graphics.h>

namespace msi::graphics {

void Graphics::render(const Context &ctx) const noexcept {
   ctx.set_draw_color(0, 0, 0, 255);
   ctx.clear();

   ctx.set_draw_color(255, 255, 0, 255);
   ctx.draw_line(20, 20, 80, 20);
   ctx.set_draw_color(255, 0, 255, 255);
   ctx.draw_line(80, 20, 80, 80);
   ctx.set_draw_color(0, 0, 255, 255);
   ctx.draw_line(80, 80, 20, 80);
   ctx.set_draw_color(0, 255, 255, 255);
   ctx.draw_line(20, 80, 20, 20);

   ctx.present();
}

}