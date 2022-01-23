#ifndef MSI_CARCASSONNE_TRAINING
#define MSI_CARCASSONNE_TRAINING
#include <Carcassonne/RL/DataCreator.h>
#include <Carcassonne/RL/Network.h>
#include <Carcassonne/Training/Gameplay.h>
#include <Carcassonne/Training/TrainingSet.h>
#include <Util/DataWithPromise.h>
#include <numeric>
#include <mb/int.h>

namespace carcassonne::training {

class Training {
   const mb::size m_workers_per_gpu;
   const mb::u64 m_games_count;
   const mb::size m_training_set_size;
   mb::size m_games_till_training = 350;// 1 game = 5.7 sek
   mb::size m_training_steps_till_checkpoint = 1000;
   const mb::u64 m_seed = 1000;
   std::unique_ptr<std::vector<std::promise<OneGame>>> m_promises;
   std::unique_ptr<std::vector<util::DataWithPromise<mb::u64, OneGame>>> m_data;
   bool m_running = true;
   unsigned m_rl_count;
   unsigned m_trees_count = 1;
   std::mt19937 &m_generator;
   std::vector<OneGame> m_full_training_data;


 public:
   Training(mb::u64 seed, std::mt19937& generator);
   ~Training();

   void run();

   void train_network(const std::array<OneGame, ai::rl::g_batch_size>& batch);

   constexpr void stop() {
      m_running = false;
   }
};

}// namespace carcassonne::training

#endif// MSI_CARCASSONNE_TRAINING
