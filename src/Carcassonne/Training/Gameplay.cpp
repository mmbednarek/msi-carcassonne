#include <Carcassonne/Training/Gameplay.h>

namespace carcassonne::training {

Gameplay::Gameplay(
   int player_count,
   uint64_t seed,
   unsigned trees_count,
   std::promise<training::OneGame> *promise_ptr
)
 : m_game(player_count, seed)
 , m_rollout(player_count, seed)
 , m_trees_count(trees_count)
 , m_promise_ptr(promise_ptr)
{
   m_game.on_next_move([this](carcassonne::IGame &game, carcassonne::Player player, carcassonne::FullMove move) {
      m_rollout.register_move(move);
      auto tile = game.tile_set()[game.move_index()];
      spdlog::info("game: Player {} places tile {} at location ({}, {}, {}), {} remaining", player_to_string(player), tile, move.x, move.y, move.rotation, game.tile_set().size() - game.move_index() - 2);
      if (move.ignored_figure) {
         spdlog::info("game: Player {} did not place any figure", player_to_string(player));
      } else {
         spdlog::info("game: Player {} placed figure at direction {}.", player_to_string(player), direction_to_string(move.direction));
      }
   });
}

std::string_view Gameplay::player_to_string(carcassonne::Player player) {
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

std::string_view Gameplay::direction_to_string(carcassonne::Direction dir) {
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
}
