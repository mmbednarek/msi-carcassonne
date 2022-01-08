#ifndef MSI_CARCASSONNE_TRAINING
#define MSI_CARCASSONNE_TRAINING
#include <Carcassonne/Training/TrainingSet.h>
#include <Carcassonne/Training/Gameplay.h>
#include <Util/DataWithPromise.h>
#include <Carcassonne/RL/DataCreator.h>
#include <numeric>

namespace carcassonne::training {

class Training {
   mb::size m_workers_per_gpu = 1;
   mb::size m_games_count = 1;//25000;
   mb::size m_training_set_size = 500'000;
   mb::size m_games_till_training = 350; // 1 game = 5.7 sek
   mb::size m_training_steps_till_checkpoint = 1000;
   uint64_t m_seed = 1000;
   unsigned m_rl_count = 2;
   unsigned m_trees_count = 1;
   std::vector<OneGame> m_new_training_data;
   std::unique_ptr<ai::rl::thread_pool> m_workers_pool = nullptr;
   std::unique_ptr<ai::rl::data_creator_pool> m_data_creator_pool = nullptr;

 public:
   Training(uint64_t seed)
    : m_seed(seed)
    , m_workers_pool(std::make_unique<ai::rl::thread_pool>(m_workers_per_gpu))
    , m_data_creator_pool(std::make_unique<ai::rl::data_creator_pool>(m_rl_count, m_workers_pool, m_trees_count))
   {
   }

   void run() {
      std::vector<uint64_t> seeds{m_games_count};
      uint64_t last_seed = 0;
      while (true) {
         spdlog::warn("seed = {}", last_seed);
         std::iota(seeds.begin(), seeds.end(), last_seed);
         std::vector<std::promise<OneGame>> promises{m_games_count};
         std::vector<util::DataWithPromise<uint64_t, OneGame>> data{m_games_count};
         std::transform(seeds.begin(), seeds.end(), promises.begin(), data.begin(),
            [](uint64_t& s, std::promise<OneGame>& og){ return util::DataWithPromise<uint64_t, OneGame>{ &og, &s }; } );
         for (int i = 0; i < m_games_count; ++i) {
            m_data_creator_pool->submit(data[i]);
         }
         for (int i = 0; i < m_games_count; ++i) {
            m_new_training_data.emplace_back(data[i].promise->get_future().get());
         }
         train_network();
         create_training_checkpoint();
         last_seed += m_games_count;
      }    
   }
   void train_network() {}
   void create_training_checkpoint() {}
};

}

#endif // MSI_CARCASSONNE_TRAINING
