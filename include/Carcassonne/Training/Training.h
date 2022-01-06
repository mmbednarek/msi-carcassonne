#ifndef MSI_CARCASSONNE_TRAINING
#define MSI_CARCASSONNE_TRAINING
#include <Carcassonne/Training/TrainingSet.h>
#include <Carcassonne/Training/Gameplay.h>
#include <Util/DataWithPromise.h>

namespace carcassonne::training {

class Training {
   mb::size m_workers_per_gpu = 2;
   mb::size m_parallel_games = 96;
   mb::size m_training_set_size = 500'000;
   mb::size m_games_till_training = 350; // 1 game = 280 sek
   mb::size m_training_steps_till_checkpoint = 1000;
   uint64_t m_seed = 1000;
   ai::rl::thread_pool m_workers_pool;

 public:
   Training(uint64_t seed) : m_seed(seed) {} 

   void run() {
      std::vector<uint64_t> seeds;
      std::vector<std::thread> games_threads;
      std::vector<std::promise<OneGame>> promises{m_parallel_games};
      std::vector<util::DataWithPromise<uint64_t, OneGame>> data{m_parallel_games};
      std::transform(seeds.begin(), seeds.end(), promises.begin(), data.begin(), 
         [](uint64_t& s, std::promise<OneGame>& og){ return util::DataWithPromise<uint64_t, OneGame>{ &og, &s }; } );
      for (int i = 0; i < m_parallel_games; ++i) {
         // ctx_ptr->workers_pool.submit(data[i]);
      }
      for (int i = 0; i < m_parallel_games; ++i) {
         data[i].promise->get_future().get();
      }
      
      while (true) {
         for (mb::size i = 0; i < m_parallel_games; ++i) {
            games_threads.push_back(std::thread{produce_game, m_seed, 2, std::ref(m_workers_pool)});
         }
         train_network();
         create_training_checkpoint();
      }    
   }

   static void produce_game(int seed, int rl_count, carcassonne::ai::rl::thread_pool& workers_pool) {
      Gameplay gameplay(rl_count, seed);

      std::mt19937 generator(seed);
      
      for (int i = 0; i < rl_count; ++i) {
         gameplay.add_rl_player(generator, workers_pool);
      }

      gameplay.add_random_player(generator);

      gameplay.run();

   } 
   void train_network() {}
   void create_training_checkpoint() {}
};

}

#endif // MSI_CARCASSONNE_TRAINING
