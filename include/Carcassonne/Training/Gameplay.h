#ifndef MSI_CARCASSONNE_TRAINING_GAMEPLAY
#define MSI_CARCASSONNE_TRAINING_GAMEPLAY
#include <Carcassonne/AI/DeepRLPlayer.h>
#include <Carcassonne/AI/RandomPlayer.h>
#include <Carcassonne/Training/TrainingSet.h>
#include <Carcassonne/Encoder/Rollout.h>
#include <Carcassonne/Game/Game.h>
#include <google/protobuf/text_format.h>
#define SPDLOG_FMT_EXTERNAL
#include <spdlog/spdlog.h>

namespace carcassonne::training {

class Gameplay {
   carcassonne::game::Game m_game;
   std::vector<std::unique_ptr<carcassonne::ai::DeepRLPlayer>> m_rl_players;
   std::vector<carcassonne::ai::RandomPlayer<>> m_random_players;
   carcassonne::Player m_next_player = carcassonne::Player::Black;
   carcassonne::encoder::Rollout m_rollout;
   unsigned m_trees_count;
   std::promise<training::OneGame> *m_promise_ptr = nullptr;

 public:
   Gameplay(
      int player_count,
      uint64_t seed,
      unsigned trees_count,
      std::promise<training::OneGame> *promise_ptr
   );

   inline void add_rl_player(
      std::mt19937 &generator,
      std::unique_ptr<carcassonne::ai::rl::thread_pool>& workers_pool
   )
   {
      m_rl_players.emplace_back(std::make_unique<carcassonne::ai::DeepRLPlayer>(m_game, m_next_player, generator, workers_pool, m_trees_count));
      std::this_thread::sleep_for(std::chrono::milliseconds(5000));
      m_next_player = carcassonne::next_player(m_next_player, 4);
   }

   inline void add_random_player(std::mt19937 &generator) {
      m_random_players.emplace_back(generator, m_next_player);
      m_random_players.back().await_turn(m_game);
      m_next_player = carcassonne::next_player(m_next_player, 4);
   }

   inline void add_watcher() {
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

   void save(std::string_view filename) {
      m_rollout.save_rollout(filename);
   }

   void submit_game_data() {
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
      try {
         m_promise_ptr->set_value(std::move(m_game.training_data()));
      } catch (std::future_error& err) {
         spdlog::error("set_value error: {}", err.what());
      }
   }

   void run() {
      m_game.start();
      while (m_game.move_index() < carcassonne::g_max_moves-1) {
         m_game.update(0);
      }
      submit_game_data();
   }

private:
   std::string_view player_to_string(carcassonne::Player player);
   std::string_view direction_to_string(carcassonne::Direction dir);
};

} // namespace carcassonne::training

#endif // MSI_CARCASSONNE_TRAINING_GAMEPLAY
