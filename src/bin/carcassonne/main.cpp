#include <Carcassonne/AI/RandomPlayer.h>
#include <Carcassonne/AI/MCTSPlayer.h>
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

constexpr bool player_range_ok(int count) {
   return count >= 0 && count <= 4;
}

int main() {
   using carcassonne::frontend::Status;

   auto game_seed = std::stoull(mb::getenv("C_SEED").unwrap("9"));
   auto human_player_count = std::stoi(mb::getenv("C_HUMAN_PLAYERS").unwrap("1"));
   auto random_ai_player_count = std::stoi(mb::getenv("C_AI_RANDOM_PLAYERS").unwrap("0"));
   auto mcts_ai_player_count = std::stoi(mb::getenv("C_AI_MCTS_PLAYERS").unwrap("1"));

   if (!player_range_ok(human_player_count)) {
      fmt::print(stderr, "invalid human player count (0-4): {}", human_player_count);
      return 1;
   }

   if (!player_range_ok(random_ai_player_count)) {
      fmt::print(stderr, "invalid random ai player count (0-4): {}", random_ai_player_count);
      return 1;
   }

   if (!player_range_ok(mcts_ai_player_count)) {
      fmt::print(stderr, "invalid random ai player count (0-4): {}", random_ai_player_count);
      return 1;
   }

   if (!player_range_ok(human_player_count + random_ai_player_count + mcts_ai_player_count)) {
      fmt::print(stderr, "invalid player count (0-4): {}", human_player_count + random_ai_player_count + mcts_ai_player_count);
      return 1;
   }

   graphics::Config cfg{
           .width = std::stoi(mb::getenv("WND_WIDTH").unwrap("800")),
           .height = std::stoi(mb::getenv("WND_HEIGHT").unwrap("600")),
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

   carcassonne::game::Game game(human_player_count + random_ai_player_count + mcts_ai_player_count, game_seed);

   auto human_players = std::accumulate(
           carcassonne::g_players.begin(),
           carcassonne::g_players.begin() + human_player_count + mcts_ai_player_count,
           carcassonne::PlayerAssignment::None,
           [](carcassonne::PlayerAssignment players, carcassonne::Player p) {
              return players | carcassonne::player_to_assignment(p);
           });
   carcassonne::frontend::GameView view(game, human_players);

   std::vector<std::unique_ptr<carcassonne::ai::RandomPlayer>> random_players(random_ai_player_count);
   std::vector<std::unique_ptr<carcassonne::ai::MCTSPlayer>> mcts_players(mcts_ai_player_count);
   std::transform(carcassonne::g_players.begin() + human_player_count, carcassonne::g_players.begin() + (human_player_count + random_ai_player_count), random_players.begin(), [&game](carcassonne::Player p) {
      return std::make_unique<carcassonne::ai::RandomPlayer>(game, p);
   });
   std::transform(carcassonne::g_players.begin() + human_player_count + random_ai_player_count, carcassonne::g_players.begin() + (human_player_count + random_ai_player_count + mcts_ai_player_count), mcts_players.begin(), [&game](carcassonne::Player p) {
      return std::make_unique<carcassonne::ai::MCTSPlayer>(game, p);
   });

   constexpr double dt = 1000.0 / 60.0;
   auto prev_time = static_cast<double>(now_milis());
   auto dt_accum = 0.0;

   game.start();

   while (view.status() != Status::Quitting) {
      auto now = static_cast<double>(now_milis());
      auto diff = now - prev_time;
      prev_time = now;

      dt_accum += diff;
      while (dt_accum > dt) {
         dt_accum -= dt;
         game.update(dt);
         view.update(dt);
      }
      carcassonne::frontend::ResourceManager::the().pre_render_hook(surface);
      view.render(surface.context());
      carcassonne::frontend::ResourceManager::the().post_render_hook();
      input::handle_events(view);
   }
   return 0;
}
