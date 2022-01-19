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
}

void Gameplay::add_random_player(std::mt19937 &generator) {
   m_random_players.emplace_back(generator, m_next_player);
   m_random_players.back().await_turn(m_game);
   m_next_player = ::carcassonne::next_player(m_next_player, 4);
}

void Gameplay::add_watcher() {
   m_game.on_next_move([this](carcassonne::IGame &game, carcassonne::Player player, carcassonne::FullMove move) {
      m_rollout.register_move(move);
      auto tile = game.tile_set()[game.move_index()];
      spdlog::info("{} places tile {} at location ({}, {}, {}), {} remaining", player_to_string(player), tile, move.x, move.y, move.rotation, game.tile_set().size() - game.move_index() - 1);
      if (move.ignored_figure) {
         spdlog::info("{} did not place any figure", player_to_string(player));
      } else {
         spdlog::info("{} placed figure at direction {}.", player_to_string(player), direction_to_string(move.direction));
      }
   });
}

void Gameplay::save(std::string_view filename) {
   m_rollout.save_rollout(filename);
}

void Gameplay::submit_game_data() {
   auto winner_player = Player::Black;
   if (!m_game.scores().is_empty()) {
      auto winner_it = std::max_element(m_game.scores().begin(), m_game.scores().end(), [](PlayerScore lhs, PlayerScore rhs) {
         return lhs.score < rhs.score;
      });
      winner_player = winner_it->player;
   }
   for (auto& record : m_game.training_data()) {
      if (winner_player == record.player) {
         record.reward = 1.0;
         continue;
      }
      record.reward = -1.0;
   }
   if (nullptr == m_promise_ptr) spdlog::error("nullptr == m_promise_ptr");
   if (0 == m_game.training_data().size()) spdlog::error("0 == m_game.training_data().size()");
   try {
      spdlog::warn("m_promise_ptr->set_value");
      m_promise_ptr->set_value(std::move(m_game.training_data()));
      spdlog::warn("m_promise_ptr->set_value OK");
   } catch (std::future_error& err) {
      spdlog::error("set_value error: {}", err.what());
   }
}

void Gameplay::run() {
   m_game.start();
   while (m_game.move_index() < carcassonne::g_max_moves-1) {
      m_game.update(0);
   }
   submit_game_data();
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
