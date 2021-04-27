#include <Carcassonne/Frontend/GameView.h>
#include <Carcassonne/Frontend/Resource.h>
#include <Carcassonne/Game/Game.h>
#include <Graphics/Surface.h>
#include <Input/Input.h>
#include <chrono>
#include <fmt/core.h>
#include <mb/core.h>

uint64_t now_milis() {
   return std::chrono::duration_cast<std::chrono::milliseconds>(
                  std::chrono::system_clock::now().time_since_epoch())
           .count();
}

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

   constexpr double dt = 1000.0 / 60.0;
   auto prev_time = static_cast<double>(now_milis());
   auto dt_accum = 0.0;

   while (view.status() != Status::Quitting) {
      auto now = static_cast<double>(now_milis());
      auto diff = now - prev_time;
      prev_time = now;

      dt_accum += diff;
      while (dt_accum > dt) {
         dt_accum -= dt;
         view.update(dt);
      }

      view.render(surface.context());
      input::handle_events(view);
   }
   return 0;
}
