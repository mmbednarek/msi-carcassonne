#include <Carcassonne/Decoder/Proto.h>
#include <Carcassonne/Encoder/Proto.h>
#include <Carcassonne/Encoder/Rollout.h>
#include <Carcassonne/Game/Game.h>
#include <fstream>

namespace carcassonne::encoder {

Rollout::Rollout(int player_count, std::uint64_t seed) {
   m_rollout.set_player_count(player_count);
   m_rollout.set_seed(seed);
}

void Rollout::register_move(const FullMove &move) {
   m_rollout.mutable_moves()->Add(move_to_proto(move));
}

void Rollout::save_rollout(std::string_view filename) const {
   std::ofstream stream(filename.data());
   m_rollout.SerializePartialToOstream(&stream);
}

void Rollout::load_rollout(std::string_view filename) {
   std::ifstream stream(filename.data());
   m_rollout.ParseFromIstream(&stream);
}

std::unique_ptr<game::Game> Rollout::as_game() {
   auto game = std::make_unique<game::Game>(m_rollout.player_count(), m_rollout.seed());
   auto player = Player::Black;

   for (const auto &move : m_rollout.moves()) {
      if (move.x() == 0 && move.y() == 0)
         continue;
      auto game_move = game->new_move(player);
      if (auto res = game_move->place_tile_at(move.x(), move.y(), move.rotation()); !res.ok()) {
         return {};
      }

      if (move.skip_figure()) {
         if (game_move->phase() != MovePhase::Done) {
            if (auto res = game_move->ignore_figure(); !res.ok()) {
               return {};
            }
         }

         player = next_player(player, m_rollout.player_count());
         continue;
      }

      if (auto res = game_move->place_figure(decoder::direction_from_proto(move.direction())); !res.ok()) {
         return {};
      }

      player = next_player(player, m_rollout.player_count());
   }

   return game;
}

void Rollout::encode(std::ostream &filename) const {
   m_rollout.SerializePartialToOstream(&filename);
}

void Rollout::decode(std::istream &filename) {
   m_rollout.ParseFromIstream(&filename);
}

}// namespace carcassonne::encoder