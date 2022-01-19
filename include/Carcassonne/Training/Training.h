#ifndef MSI_CARCASSONNE_TRAINING
#define MSI_CARCASSONNE_TRAINING
#include <Carcassonne/RL/DataCreator.h>
#include <Carcassonne/RL/Network.h>
#include <Carcassonne/Training/Gameplay.h>
#include <Carcassonne/Training/TrainingSet.h>
#include <Util/DataWithPromise.h>
#include <numeric>

namespace carcassonne::training {

class Training {
   mb::size m_workers_per_gpu = 1;
   uint64_t m_games_count = 1;//25000;
   mb::size m_training_set_size = 500'000;
   mb::size m_games_till_training = 350;// 1 game = 5.7 sek
   mb::size m_training_steps_till_checkpoint = 1000;
   uint64_t m_seed = 1000;
   bool m_running = true;
   unsigned m_rl_count = 2;
   unsigned m_trees_count = 1;
   std::vector<OneGame> m_new_training_data;
   std::unique_ptr<ai::rl::thread_pool> m_workers_pool = nullptr;
   std::unique_ptr<ai::rl::data_creator_pool> m_data_creator_pool = nullptr;


 public:
   Training(uint64_t seed);

   void run();

   void train_network();

   constexpr void stop() {
      m_running = false;
   }
};

}// namespace carcassonne::training

#endif// MSI_CARCASSONNE_TRAINING
