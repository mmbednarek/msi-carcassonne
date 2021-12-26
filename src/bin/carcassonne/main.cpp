#include "Carcassonne/AI/DeepRLPlayer.h"
#include <Carcassonne/AI/HeuristicPlayer.h>
#include <Carcassonne/AI/MCTSPlayer.h>
#include <Carcassonne/AI/RandomPlayer.h>
#include <Carcassonne/Frontend/GameView.h>
#include <Carcassonne/Frontend/Resource.h>
#include <Carcassonne/Game/Game.h>
#include "Carcassonne/Game/Game_GPU.h"
#include <Graphics/Surface.h>
#include <Input/Input.h>
#include <chrono>
#include <fmt/core.h>
#include <google/protobuf/text_format.h>
#include <mb/core.h>
#include <spdlog/spdlog.h>

uint64_t now_milis() {
   return std::chrono::duration_cast<std::chrono::milliseconds>(
                  std::chrono::system_clock::now().time_since_epoch())
           .count();
}

constexpr bool player_range_ok(int count) {
   return count >= 0 && count <= 4;
}


mb::result<std::unique_ptr<carcassonne::rl::Network>> load_network() {
   caffe::Caffe::set_mode(caffe::Caffe::GPU);

   caffe::SolverParameter solver_param;
   caffe::ReadSolverParamsFromTextFileOrDie("./solver.prototxt", &solver_param);

   caffe::NetParameter net_parameter;
   std::ifstream t("./net_tic_tac_6_4_2_res_block.prototxt");
   std::string model((std::istreambuf_iterator<char>(t)),
                     std::istreambuf_iterator<char>());
   bool success = google::protobuf::TextFormat::ParseFromString(model, &net_parameter);
   if (!success) {
      return mb::error("could not parse protobuf file");
   }

   net_parameter.mutable_state()->set_phase(caffe::TRAIN);

   return std::make_unique<carcassonne::rl::Network>(net_parameter, solver_param);
}

std::string_view player_to_string(carcassonne::Player player) {
   switch (player) {
   case carcassonne::Player::Black:
      return "black";
   case carcassonne::Player::Blue:
      return "blue";
   case carcassonne::Player::Yellow:
      return "yellow";
   case carcassonne::Player::Red:
      return "red";
   }
   return "";
}

std::string_view direction_to_string(carcassonne::Direction dir) {
   switch(dir) {
   case carcassonne::Direction::North:
      return "North";
   case carcassonne::Direction::East:
      return "East";
   case carcassonne::Direction::South:
      return "South";
   case carcassonne::Direction::West:
      return "West";
   case carcassonne::Direction::Middle:
      return "Middle";
   case carcassonne::Direction::NorthEast:
      return "NorthEast";
   case carcassonne::Direction::EastNorth:
      return "EastNorth";
   case carcassonne::Direction::EastSouth:
      return "EastSouth";
   case carcassonne::Direction::SouthEast:
      return "SouthEast";
   case carcassonne::Direction::SouthWest:
      return "SouthWest";
   case carcassonne::Direction::WestSouth:
      return "WestSouth";
   case carcassonne::Direction::WestNorth:
      return "WestNorth";
   case carcassonne::Direction::NorthWest:
      return "NorthWest";
   }
   return "";
}

class Gameplay {
   caffe::Caffe::Brew m_device_type = caffe::Caffe::GPU;
   std::unique_ptr<carcassonne::IGame> m_igame;
   std::vector<carcassonne::ai::DeepRLPlayer> m_rl_players;
   std::vector<carcassonne::ai::RandomPlayer<>> m_random_players;
   carcassonne::Player m_next_player = carcassonne::Player::Black;

 public:
   Gameplay(int player_count, uint64_t seed, int device_id) {
      if (device_id > 7) m_device_type = caffe::Caffe::CPU;
      if (m_device_type == caffe::Caffe::CPU) {
         // m_igame = std::unique_ptr<carcassonne::game::Game>(new carcassonne::game::Game{player_count, seed});
         m_igame = std::make_unique<carcassonne::game::Game>(player_count, seed);
      } else {
         m_igame = std::make_unique<carcassonne::game::Game_GPU>(player_count, seed);
      }
      m_igame->on_next_move([] (std::unique_ptr<carcassonne::IGame> game, carcassonne::Player player, carcassonne::FullMove move) {
         auto tile = game->tile_set()[game->move_index()];
         spdlog::info("game: Player {} places tile {} at location ({}, {}, {})", player_to_string(player), tile, move.x, move.y, move.rotation);
         if (move.ignored_figure) {
            spdlog::info("game: Player {} did not place any figure", player_to_string(player));
         } else {
            spdlog::info("game: Player {} placed figure at direction {}.", player_to_string(player), direction_to_string(move.direction));
         }
      });
   }
};

int main() {
   using carcassonne::frontend::Status;

   auto game_seed = std::stoull(mb::getenv("C_SEED").unwrap("9"));
   auto human_player_count = std::stoi(mb::getenv("C_HUMAN_PLAYERS").unwrap("1"));
   auto random_ai_player_count = std::stoi(mb::getenv("C_AI_RANDOM_PLAYERS").unwrap("0"));
   auto mcts_ai_player_count = std::stoi(mb::getenv("C_AI_MCTS_PLAYERS").unwrap("0"));
   auto heuristic_ai_player_count = std::stoi(mb::getenv("C_AI_HEURISTIC_PLAYERS").unwrap("0"));
   auto rl_ai_player_count = std::stoi(mb::getenv("C_AI_RL_PLAYERS").unwrap("1"));

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

   auto player_count = human_player_count +
                       random_ai_player_count +
                       heuristic_ai_player_count +
                       mcts_ai_player_count +
                       rl_ai_player_count;

   if (!player_range_ok(player_count)) {
      fmt::print(stderr, "invalid player count (0-4): {}", human_player_count + random_ai_player_count + mcts_ai_player_count + heuristic_ai_player_count);
      return 1;
   }

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

   std::unique_ptr<carcassonne::IGame> game = std::make_unique<carcassonne::game::Game>(player_count, game_seed);

   auto human_players = std::accumulate(
           carcassonne::g_players.begin(),
           carcassonne::g_players.begin() + human_player_count,
           carcassonne::PlayerAssignment::None,
           [](carcassonne::PlayerAssignment players, carcassonne::Player p) {
              return players | carcassonne::player_to_assignment(p);
           });
   carcassonne::frontend::GameView view(game, human_players);

   std::random_device random_device;
   std::mt19937 random_generator(random_device());

   auto player_id = human_player_count;

   std::vector<carcassonne::ai::RandomPlayer<>> random_players;
   random_players.reserve(random_ai_player_count);
   std::transform(carcassonne::g_players.begin() + player_id, carcassonne::g_players.begin() + (player_id + random_ai_player_count), std::back_inserter(random_players), [&random_generator](carcassonne::Player p) {
      return carcassonne::ai::RandomPlayer<>(random_generator, p);
   });
   for (auto &player : random_players) {
      player.await_turn(game);
   }

   player_id += random_ai_player_count;

   std::vector<carcassonne::ai::HeuristicPlayer> heuristic_players;
   heuristic_players.reserve(heuristic_ai_player_count);
   std::transform(carcassonne::g_players.begin() + player_id, carcassonne::g_players.begin() + (player_id + heuristic_ai_player_count), std::back_inserter(heuristic_players), [](carcassonne::Player p) {
      return carcassonne::ai::HeuristicPlayer(p);
   });
   for (auto &player : heuristic_players) {
      player.await_turn(game);
   }

   player_id += heuristic_ai_player_count;

   std::vector<std::unique_ptr<carcassonne::ai::MCTSPlayer>> mcts_players(mcts_ai_player_count);
   std::transform(carcassonne::g_players.begin() + player_id, carcassonne::g_players.begin() + (player_id + mcts_ai_player_count), mcts_players.begin(), [&game](carcassonne::Player p) {
      return std::make_unique<carcassonne::ai::MCTSPlayer>(game, p, carcassonne::ai::SimulationType::Heuristic);
   });

   auto net_res = load_network();
   if (!net_res.ok()) {
      return EXIT_FAILURE;
   }
   auto net = net_res.unwrap();

   player_id += mcts_ai_player_count;

   std::vector<std::unique_ptr<carcassonne::ai::DeepRLPlayer>> rl_players(rl_ai_player_count);
   std::transform(carcassonne::g_players.begin() + player_id, carcassonne::g_players.begin() + (player_id + rl_ai_player_count), rl_players.begin(), [&game, &net](carcassonne::Player p) {
      return std::make_unique<carcassonne::ai::DeepRLPlayer>(game, p, *net);
   });

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
