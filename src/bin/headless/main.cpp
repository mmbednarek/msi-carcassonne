#include "Carcassonne/AI/DeepRLPlayer.h"
#include "Carcassonne/AI/RandomPlayer.h"
#include "Carcassonne/Game/Game.h"
#include <boost/program_options.hpp>
#define SPDLOG_FMT_EXTERNAL
#include <spdlog/spdlog.h>
#include <google/protobuf/text_format.h>
// #include <cudatest/cudatest.h>

namespace po = boost::program_options;

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
   carcassonne::game::Game m_game;
   std::vector<carcassonne::ai::DeepRLPlayer> m_rl_players;
   std::vector<carcassonne::ai::RandomPlayer<>> m_random_players;
   carcassonne::Player m_next_player = carcassonne::Player::Black;

 public:
   Gameplay(int player_count, uint64_t seed) : m_game(player_count, seed) {
      m_game.on_next_move([] (carcassonne::IGame &game, carcassonne::Player player, carcassonne::FullMove move) {
         auto tile = game.tile_set()[game.move_index()];
         spdlog::info("game: Player {} places tile {} at location ({}, {}, {})", player_to_string(player), tile, move.x, move.y, move.rotation);
         if (move.ignored_figure) {
            spdlog::info("game: Player {} did not place any figure", player_to_string(player));
         } else {
            spdlog::info("game: Player {} placed figure at direction {}.", player_to_string(player), direction_to_string(move.direction));
         }
      });
   }

   void add_rl_player(carcassonne::rl::Network &net) {
      m_rl_players.emplace_back(m_game, m_next_player, net);
      m_next_player = carcassonne::next_player(m_next_player, 4);
   }

   void add_random_player(std::mt19937 &generator) {
      m_random_players.emplace_back(generator, m_next_player);
      m_random_players.back().await_turn(m_game);
      m_next_player = carcassonne::next_player(m_next_player, 4);
   }

   void run() {
      m_game.start();
      while(m_game.move_index() < carcassonne::g_max_moves) {
         m_game.update(0);
      }
   }
};

mb::result<std::unique_ptr<carcassonne::rl::Network>> load_network() {
   caffe::Caffe::set_mode(caffe::Caffe::GPU);
   // caffe::Caffe::SetDevice(6);

   caffe::SolverParameter solver_param;
   caffe::ReadSolverParamsFromTextFileOrDie("./proto/solver.prototxt", &solver_param);

   caffe::NetParameter net_parameter;
   std::ifstream t("./proto/net_tic_tac_6_4_2_res_block.prototxt");
   std::string model((std::istreambuf_iterator<char>(t)),
                     std::istreambuf_iterator<char>());
   bool success = google::protobuf::TextFormat::ParseFromString(model, &net_parameter);
   if (!success) {
      return mb::error("could not parse protobuf file");
   }

   net_parameter.mutable_state()->set_phase(caffe::TRAIN);

   return std::make_unique<carcassonne::rl::Network>(net_parameter, solver_param);
}

int main(int argc, char **argv) {
   // spdlog::debug("cudatest start\n");
   // cudatest::test_cuda();
   // spdlog::debug("cudatest complete\n");
   // return 0;
   po::options_description desc("carcassonne headless");
   desc.add_options()
           ("help", "")
           ("seed,s", po::value<int>()->default_value(10), "seed of a game")
           ("rl-ai-count,l", po::value<int>()->default_value(1), "number of rl agents")
           ("random-ai-count,r", po::value<int>()->default_value(1), "number of random agents")
           ("verbose,v", "verbose output");

   po::variables_map values;
   po::store(po::command_line_parser(argc, argv).options(desc).run(), values);

   auto seed = values["seed"].as<int>();
   auto rl_count = values["rl-ai-count"].as<int>();
   auto random_count = values["random-ai-count"].as<int>();

   if (values.contains("verbose")) {
      spdlog::set_level(spdlog::level::debug);
   }

   auto total_player_count = rl_count + random_count;
   if (total_player_count > 4) {
      spdlog::error("player count may not exceed 4");
      return EXIT_FAILURE;
   }

   Gameplay gameplay(total_player_count, seed);

   auto network_res = load_network();
   if (!network_res.ok()) {
      spdlog::error("could not load network: {}", network_res.msg());
      return EXIT_FAILURE;
   }
   auto network = network_res.unwrap();

   for (int i = 0; i < rl_count; ++i) {
      gameplay.add_rl_player(*network);
   }

   std::mt19937 generator(seed);
   gameplay.add_random_player(generator);

   gameplay.run();
}