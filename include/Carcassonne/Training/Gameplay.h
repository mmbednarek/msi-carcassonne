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
   std::vector<carcassonne::ai::DeepRLPlayer> m_rl_players;
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

   void add_rl_player(
      std::mt19937 &generator,
      std::unique_ptr<carcassonne::ai::rl::thread_pool>& workers_pool
   )
   {
      m_rl_players.emplace_back(m_game, m_next_player, generator, workers_pool, m_trees_count);
      std::this_thread::sleep_for(std::chrono::seconds(5));
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

   void submit_game_data() {
      auto winner_it = std::max_element(m_game.scores().begin(), m_game.scores().end(), [](PlayerScore lhs, PlayerScore rhs) {
         return lhs.score < rhs.score;
      });
      for (auto& record : m_game.training_data()) {
         if (winner_it->player == record.player) {
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
      while (m_game.move_index() < carcassonne::g_max_moves) {
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
