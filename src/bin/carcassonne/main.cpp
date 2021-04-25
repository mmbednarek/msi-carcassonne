#include <Carcassonne/Game/Game.h>
#include <Carcassonne/Graphics/Resource.h>
#include <Carcassonne/Graphics/Surface.h>
#include <Carcassonne/Input/Input.h>
#include <fmt/core.h>
#include <mb/core.h>

int main() {
   using carcassonne::game::Status;

   carcassonne::graphics::Config cfg{
           .width = std::stoi(mb::getenv("WND_WIDTH").unwrap("800")),
           .height = std::stoi(mb::getenv("WND_HEIGHT").unwrap("600")),
   };
   auto surface_re = carcassonne::graphics::Surface::create(cfg);
   if (!surface_re.ok()) {
      fmt::print(stderr, "could not initialise graphic API: {}", surface_re.msg());
      return 1;
   }
   auto surface = surface_re.unwrap();

   auto resource_re = carcassonne::graphics::ResourceManager::the().load_resources(surface);
   if (!resource_re.ok()) {
      fmt::print(stderr, "could not load all resources: {}", resource_re.msg());
      return 1;
   }

   carcassonne::game::Game game;
   while (game.status() != Status::Quitting) {
      game.update();// TODO: Add fixed timestep
      game.render().render(surface.context());
      carcassonne::input::handle_events(game.event_manager());
   }
   return 0;
}
