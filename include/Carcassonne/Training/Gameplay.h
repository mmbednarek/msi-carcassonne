#ifndef MSI_CARCASSONNE_TRAINING_GAMEPLAY
#define MSI_CARCASSONNE_TRAINING_GAMEPLAY
#include <Carcassonne/AI/DeepRLPlayer.h>
#include <Carcassonne/AI/RandomPlayer.h>
#include <Carcassonne/Encoder/Rollout.h>
#include <Carcassonne/Game/Game.h>
#include <google/protobuf/text_format.h>
#define SPDLOG_FMT_EXTERNAL
#include <spdlog/spdlog.h>

namespace carcassonne::training {

class Gameplay {
   carcassonne::game::Game m_game;
   std::vector<carcassonne::ai::DeepRLPlayer> m_rl_players;
   std::vector<carcassonne::ai::RandomPlayer<>> m_random_players;
   carcassonne::Player m_next_player = carcassonne::Player::Black;
   carcassonne::encoder::Rollout m_rollout;

 public:
   Gameplay(int player_count, uint64_t seed) : m_game(player_count, seed),
                                               m_rollout(player_count, seed) {
      m_game.on_next_move([this](carcassonne::IGame &game, carcassonne::Player player, carcassonne::FullMove move) {
         m_rollout.register_move(move);
         auto tile = game.tile_set()[game.move_index()];
         spdlog::info("game: Player {} places tile {} at location ({}, {}, {})", player_to_string(player), tile, move.x, move.y, move.rotation);
         if (move.ignored_figure) {
            spdlog::info("game: Player {} did not place any figure", player_to_string(player));
         } else {
            spdlog::info("game: Player {} placed figure at direction {}.", player_to_string(player), direction_to_string(move.direction));
         }
      });
   }

   void add_rl_player(std::mt19937 &generator, carcassonne::ai::rl::thread_pool& workers_pool) {
      m_rl_players.emplace_back(m_game, m_next_player, generator, workers_pool);
      m_next_player = carcassonne::next_player(m_next_player, 4);
   }

   void add_random_player(std::mt19937 &generator) {
      m_random_players.emplace_back(generator, m_next_player);
      m_random_players.back().await_turn(m_game);
      m_next_player = carcassonne::next_player(m_next_player, 4);
   }

   void save(std::string_view filename) {
      m_rollout.save_rollout(filename);
   }

   void run() {
      m_game.start();
      while (m_game.move_index() < carcassonne::g_max_moves) {
         m_game.update(0);
      }
   }

private:
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
      switch (dir) {
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
};

} // namespace carcassonne::training

#endif // MSI_CARCASSONNE_TRAINING_GAMEPLAY
