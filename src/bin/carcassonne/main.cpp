#include <Carcassonne/Frontend/GameView.h>
#include <Carcassonne/Frontend/Resource.h>
#include <Carcassonne/Game/Game.h>
#include <Graphics/Surface.h>
#include <Input/Input.h>
#include <fmt/core.h>
#include <mb/core.h>

int main() {
   using carcassonne::frontend::Status;

   graphics::Config cfg{
           .width = std::stoi(mb::getenv("WND_WIDTH").unwrap("800")),
           .height = std::stoi(mb::getenv("WND_HEIGHT").unwrap("600")),
   };
   auto surface_re = graphics::Surface::create(cfg);
   if (!surface_re.ok()) {
      fmt::print(stderr, "could not initialise graphic API: {}", surface_re.msg());
      return 1;
   }
   auto surface = surface_re.unwrap();

   auto resource_re = carcassonne::frontend::ResourceManager::the().load_resources(surface);
   if (!resource_re.ok()) {
      fmt::print(stderr, "could not load all resources: {}", resource_re.msg());
      return 1;
   }

   carcassonne::game::Game game;
   carcassonne::frontend::GameView view(game);

   while (view.status() != Status::Quitting) {
      game.update();// TODO: Add fixed timestep
      view.render(surface.context());
      input::handle_events(view);
   }
   return 0;
}
