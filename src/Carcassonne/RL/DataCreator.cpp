#include <Carcassonne/RL/DataCreator.h>
// #include <Carcassonne/RL/Network.h>
// #include <Eden_resources/Ngpus_Ncpus.h>
// #include <vector>
// #include <span>
// #include <tuple>
// #include <spdlog/spdlog.h>

namespace carcassonne::ai::rl {

static void produce_game(
   uint64_t *seed,
   std::promise<training::OneGame> *promise_ptr,
   int rl_count,
   std::unique_ptr<carcassonne::ai::rl::thread_pool>& workers_pool,
   unsigned trees_count)
{
   fmt::print("og2={}={}\n", fmt::ptr(seed), *seed);
   training::Gameplay gameplay(rl_count, *seed, trees_count, promise_ptr);
   std::mt19937 generator(*seed);
   
   for (int i = 0; i < rl_count; ++i) {
      gameplay.add_rl_player(generator, workers_pool);
   }
   // gameplay.add_random_player(generator);
   gameplay.add_watcher();
   spdlog::debug("gameplay started");
   gameplay.run();
   spdlog::debug("gameplay finished");
   gameplay.save(fmt::format("gameplays/{}.proto", *seed) );
}

void data_creator_pool::worker_thread(
   uint64_t thread_id
)
{
   while (!done) {
      util::DataWithPromise<uint64_t, training::OneGame> np;
      if (simulations_queue.try_pop(np)) {
         fmt::print("POP!\n");
         produce_game(np.data_in, np.promise, m_rl_count, m_workers_pool, m_trees_count);
         // np.promise->set_value(just_forward(np.data_in));
      } else {
         std::this_thread::yield();
         std::this_thread::sleep_for(std::chrono::microseconds(500));
      }
   }
}
data_creator_pool::data_creator_pool(
   int rl_count,
   std::unique_ptr<carcassonne::ai::rl::thread_pool> &workers_pool,
   unsigned trees_count,
   uint64_t n_threads
)
   : m_rl_count(rl_count)
   , m_workers_pool(workers_pool)
   , m_trees_count(trees_count)
   , done(false)
   , joiner(threads)
   , games_per_cpu(1)
{
   uint64_t cpus_count = std::thread::hardware_concurrency();
   uint64_t thread_count = n_threads;
   if (n_threads == 0) thread_count = cpus_count * games_per_cpu;
   fmt::print("preparing {} data_generators\n", thread_count);
   try {
      for (uint64_t i = 0; i < thread_count; ++i) {
         threads.push_back(std::thread(
            &data_creator_pool::worker_thread, this, i ));
      }
      fmt::print("m_data_creator_pool prepared!\n");
   } catch (...) {
      done = true;
      throw;
   }
}

void data_creator_pool::submit(util::DataWithPromise< uint64_t, training::OneGame > np) {
   simulations_queue.push(np);
}

}// namespace carcassonne::ai::rl