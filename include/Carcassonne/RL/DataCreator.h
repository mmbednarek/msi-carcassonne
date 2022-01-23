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
   bool m_done;
   std::shared_ptr<std::condition_variable> m_cond = nullptr;
   std::mutex m_mut;
   std::vector<std::thread> m_threads;
   std::atomic<int> m_workers_up = 0;
   join_threads m_joiner;
   threadsafe_queue< util::DataWithPromise<uint64_t, training::OneGame> > m_simulations_queue;
   uint64_t m_games_per_cpu;

 public:
   void worker_thread(
      uint64_t thread_id
   );
   void done() {
      m_done = true;
      std::mutex mut;
      std::unique_lock<std::mutex> lck(mut);
      m_cond->wait(lck, [this] {
         spdlog::info("data_creator_pool_done={}, left {}",
                        (m_workers_up == 0 ? "true" : "false"), m_workers_up);
         return m_workers_up == 0;
      });
   }
   data_creator_pool(
      int rl_count,
      std::unique_ptr<carcassonne::ai::rl::thread_pool> &workers_pool,
      unsigned trees_count,
      uint64_t n_threads
   );
   data_creator_pool(const data_creator_pool&) = delete;
   ~data_creator_pool() {
      m_done = true;
   }

   void submit(util::DataWithPromise< uint64_t, training::OneGame > np);
};

}// namespace carcassonne::ai::rl

#endif // CARCASSONNE_RL_DATA_CREATOR_H
