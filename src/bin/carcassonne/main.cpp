#include <MSI/Graphics/Graphics.h>
#include <fmt/core.h>

int main() {
   msi::graphics::Config cfg{
           .width = 800,
           .height = 600,
   };
   msi::graphics::Graphics gfx;

   try {
      msi::graphics::Surface surf(cfg);
      surf.render(gfx);
   } catch(const std::runtime_error &e) {
      fmt::print(stderr, "Could not initialise graphic API: {}", e.what());
   }

   return 0;
}
