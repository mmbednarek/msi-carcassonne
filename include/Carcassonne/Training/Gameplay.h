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
   std::unique_ptr<::carcassonne::IGame> m_igame_ptr;
   std::pair<std::unique_ptr<::carcassonne::IGame>, training::OneGame> m_game_with_training_data;
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

void add_rl_player(
   std::mt19937 &generator,
   std::unique_ptr<::carcassonne::ai::rl::thread_pool>& workers_pool
)
{
   m_rl_players.emplace_back(std::make_unique<::carcassonne::ai::DeepRLPlayer>(m_game_with_training_data, m_next_player, generator, workers_pool, m_trees_count));
   std::this_thread::sleep_for(std::chrono::milliseconds(5000));
   m_next_player = ::carcassonne::next_player(m_next_player, 4);
}
   void add_random_player(std::mt19937 &generator);
   void add_watcher();
   void save(std::string_view filename);
   void run();

private:
   void submit_game_data();
   std::string_view player_to_string(carcassonne::Player player);
   std::string_view direction_to_string(carcassonne::Direction dir);
};

} // namespace carcassonne::training

#endif // MSI_CARCASSONNE_TRAINING_GAMEPLAY
