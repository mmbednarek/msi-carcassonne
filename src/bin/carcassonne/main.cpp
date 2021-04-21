#include <MSI/Game/Game.h>
#include <MSI/Graphics/Surface.h>
#include <MSI/Input/Input.h>
#include <fmt/core.h>
#include <mb/core.h>

int main() {
   using msi::game::Status;

   msi::graphics::Config cfg{
           .width = std::stoi(mb::getenv("WND_WIDTH").unwrap("800")),
           .height = std::stoi(mb::getenv("WND_HEIGHT").unwrap("600")),
   };
   auto surface_re = msi::graphics::Surface::create(cfg);
   if (!surface_re.ok()) {
      fmt::print(stderr, "could not initialise graphic API: {}", surface_re.msg());
      return 1;
   }
   auto surface = surface_re.unwrap();

   msi::game::Game game;
   while (game.status() != Status::Quitting) {
      game.update();// TODO: Add fixed timestep
      game.render().render(surface.context());
      msi::input::handle_events(game.event_manager());
   }
   return 0;
}
