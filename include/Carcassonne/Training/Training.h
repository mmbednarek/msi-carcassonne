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
   mb::size m_workers_per_gpu;
   const mb::u64 m_games_count;
   mb::size m_training_set_size = 500'000;
   mb::size m_games_till_training = 350;// 1 game = 5.7 sek
   mb::size m_training_steps_till_checkpoint = 1000;
   mb::u64 m_seed = 1000;
   std::unique_ptr<std::vector<std::promise<OneGame>>> m_promises;
   std::unique_ptr<std::vector<util::DataWithPromise<mb::u64, OneGame>>> m_data;
   bool m_running = true;
   unsigned m_rl_count;
   unsigned m_trees_count = 1;
   std::vector<OneGame> m_new_training_data;


 public:
   Training(mb::u64 seed);
   ~Training();

   void run();

   void train_network();

   constexpr void stop() {
      m_running = false;
   }
};

}// namespace carcassonne::training

#endif// MSI_CARCASSONNE_TRAINING
