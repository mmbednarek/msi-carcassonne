#ifndef CARCASSONNE_RL_DATA_CREATOR_H
#define CARCASSONNE_RL_DATA_CREATOR_H
#include <Carcassonne/RL/ThreadSafeQueue.h>
#include <Carcassonne/RL/Network.h>
#include <Carcassonne/Training/TrainingSet.h>
#include <Carcassonne/Training/Gameplay.h>
#include <Eden_resources/Ngpus_Ncpus.h>
#include <Util/DataWithPromise.h>
#include <vector>
#include <span>
#include <tuple>
#include <spdlog/spdlog.h>

namespace carcassonne::ai::rl {



static void produce_game(
   uint64_t *seed,
   std::promise<training::OneGame> *promise_ptr,
   int rl_count,
   std::unique_ptr<carcassonne::ai::rl::thread_pool>& workers_pool,
   unsigned trees_count)
{
   training::Gameplay gameplay(rl_count, *seed, trees_count, promise_ptr);
   std::mt19937 generator(*seed);
   
   for (int i = 0; i < rl_count; ++i) {
      gameplay.add_rl_player(generator, workers_pool);
   }
   // gameplay.add_random_player(generator);
   spdlog::debug("gameplay started");
   gameplay.run();
   spdlog::debug("gameplay finished");
   gameplay.save("gameplay.proto");
}

class data_creator_pool {
   int m_rl_count;
   std::unique_ptr<carcassonne::ai::rl::thread_pool> &m_workers_pool;
   unsigned m_trees_count;
   bool done;
   std::vector<std::thread> threads;
   join_threads joiner;
   threadsafe_queue< util::DataWithPromise<uint64_t, training::OneGame> > simulations_queue;
   uint64_t games_per_cpu = 1;

 public:
   void worker_thread(
      uint64_t thread_id
   )
   {
      // init
      while (!done) {
         util::DataWithPromise<uint64_t, training::OneGame> np;
         if (simulations_queue.try_pop(np)) {
            produce_game(np.data_in, np.promise, m_rl_count, m_workers_pool, m_trees_count);
            // np.promise->set_value(just_forward(np.data_in));
         } else {
            std::this_thread::yield();
            std::this_thread::sleep_for(std::chrono::microseconds(1));
         }
      }
      fmt::print("m_data_creator_pool prepared!\n");
   }
   data_creator_pool(
      int rl_count,
      std::unique_ptr<carcassonne::ai::rl::thread_pool> &workers_pool,
      unsigned trees_count,
      uint64_t pool_size
   )
    : m_rl_count(rl_count)
    , m_workers_pool(workers_pool)
    , m_trees_count(trees_count)
    , done(false)
    , joiner(threads)
   {
      uint64_t cpus_count = std::thread::hardware_concurrency();
      uint64_t thread_count = cpus_count * games_per_cpu;
      if (0 == pool_size) pool_size = thread_count;
      try {
         for (uint64_t i = 0; i < pool_size; ++i) {
            threads.push_back(std::thread(
               &data_creator_pool::worker_thread, this, i ));
         }
      } catch (...) {
         done = true;
         throw;
      }
   }
   data_creator_pool(const data_creator_pool&) = delete;
   data_creator_pool(data_creator_pool&&) = default;
   ~data_creator_pool() {
      done = true;
   }

   void submit(util::DataWithPromise< uint64_t, training::OneGame > np) {
      simulations_queue.push(np);
   }
};

}// namespace carcassonne::ai::rl

#endif // CARCASSONNE_RL_DATA_CREATOR_H
