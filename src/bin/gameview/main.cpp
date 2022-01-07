#include "Carcassonne/AI/DeepRLPlayer.h"
#include <Carcassonne/AI/HeuristicPlayer.h>
#include <Carcassonne/AI/MCTSPlayer.h>
#include <Carcassonne/AI/RandomPlayer.h>
#include <Carcassonne/Frontend/GameView.h>
#include <Carcassonne/Frontend/Resource.h>
#include <Carcassonne/Game/Game.h>
#include <Carcassonne/Encoder/Rollout.h>
#include <Graphics/Surface.h>
#include <Input/Input.h>
#include <chrono>
#include <fmt/core.h>
#include <google/protobuf/text_format.h>
#include <mb/core.h>

uint64_t now_milis() {
   return std::chrono::duration_cast<std::chrono::milliseconds>(
                  std::chrono::system_clock::now().time_since_epoch())
           .count();
}

constexpr bool player_range_ok(int count) {
   return count >= 0 && count <= 4;
}

int main() {
   using carcassonne::frontend::Status;

   auto gameplay_file = mb::getenv("GAMEPLAY_FILE").unwrap("gameplay.proto");
   carcassonne::encoder::Rollout rollout(0, 0);
   rollout.load_rollout(gameplay_file);

   graphics::Config cfg{
           .width = std::stoi(mb::getenv("WND_WIDTH").unwrap("960")),
           .height = std::stoi(mb::getenv("WND_HEIGHT").unwrap("540")),
           .title = "Carcassonne",
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

   auto game = rollout.as_game();

   carcassonne::frontend::GameView view(*game, carcassonne::PlayerAssignment::None);

   constexpr double dt = 1000.0 / 60.0;
   auto prev_time = static_cast<double>(now_milis());
   auto dt_accum = 0.0;

   game->start();

   while (view.status() != Status::Quitting) {
      auto now = static_cast<double>(now_milis());
      auto diff = now - prev_time;
      prev_time = now;

      dt_accum += diff;
      while (dt_accum > dt) {
         dt_accum -= dt;
         game->update(dt);
         view.update(dt);
      }
      carcassonne::frontend::ResourceManager::the().pre_render_hook(surface);
      view.render(surface.context());
      carcassonne::frontend::ResourceManager::the().post_render_hook();
      input::handle_events(view);
   }

   return EXIT_SUCCESS;
}
