#ifndef CARCASSONNE_RL_DATA_CREATOR_H
#define CARCASSONNE_RL_DATA_CREATOR_H
#include <Carcassonne/RL/ThreadSafeQueue.h>
#include <Carcassonne/Training/TrainingSet.h>
#include <Carcassonne/Training/Gameplay.h>
#include <Util/DataWithPromise.h>

namespace carcassonne::ai::rl {

class data_creator_pool {
   int m_rl_count;
   std::unique_ptr<carcassonne::ai::rl::thread_pool> &m_workers_pool;
   unsigned m_trees_count;
   bool done;
   std::vector<std::thread> threads;
   join_threads joiner;
   threadsafe_queue< util::DataWithPromise<uint64_t, training::OneGame> > simulations_queue;
   uint64_t games_per_cpu;

 public:
   void worker_thread(
      uint64_t thread_id
   );
   data_creator_pool(
      int rl_count,
      std::unique_ptr<carcassonne::ai::rl::thread_pool> &workers_pool,
      unsigned trees_count,
      uint64_t n_threads
   );
   data_creator_pool(const data_creator_pool&) = delete;
   data_creator_pool(data_creator_pool&&) = default;
   ~data_creator_pool() {
      done = true;
   }

   void submit(util::DataWithPromise< uint64_t, training::OneGame > np);
};

}// namespace carcassonne::ai::rl

#endif // CARCASSONNE_RL_DATA_CREATOR_H
