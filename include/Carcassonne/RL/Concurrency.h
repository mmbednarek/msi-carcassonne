#ifndef CARCASSONNE_RL_CONCURRENCY_H
#define CARCASSONNE_RL_CONCURRENCY_H
#pragma once
#include <Carcassonne/RL/Network.h>
#include <Carcassonne/RL/ThreadSafeQueue.h>
#include <Eden_resources/Ngpus_Ncpus.h>
#include <Util/DataWithPromise.h>
#include <condition_variable>
#include <span>
#include <spdlog/spdlog.h>

namespace carcassonne::ai::rl {

extern std::map<std::thread::id, std::unique_ptr<Network>> g_networks;

mb::result<std::unique_ptr<Network>> load_network(int gpu_id);

inline std::tuple<std::span<float>, float> just_forward(std::vector<float> *data_in) {
   auto &neuron_inputs = data_in;
   std::unique_ptr<Network> &network = g_networks[std::this_thread::get_id()];
   boost::shared_ptr<caffe::Blob<float>> input_data{network->solver().net()->blob_by_name("input_data")};
   boost::shared_ptr<caffe::Blob<float>> output_probabs{network->solver().net()->blob_by_name("output_probabs")};
   boost::shared_ptr<caffe::Blob<float>> output_value{network->solver().net()->blob_by_name("output_value")};
   std::copy(neuron_inputs->begin(), neuron_inputs->end(), input_data->mutable_cpu_data());
   network->solver().net()->Forward();
   static constexpr auto output_neuron_count =  g_board_width * g_board_height * 4 * 10;
   return std::make_tuple(std::span<float>(output_probabs->mutable_cpu_data(), output_neuron_count), *output_value->mutable_cpu_data());
}

class thread_pool {
   bool m_done;
   bool m_threads_finished = false;
   std::vector<std::thread> m_threads;
   join_threads m_joiner;
   std::shared_ptr<std::condition_variable> m_cond = nullptr;
   int m_workers_up = 0;
   
   threadsafe_queue<util::DataWithPromise< std::vector<float>, std::tuple<std::span<float>, float> >> work_queue;
   void worker_thread(int gpu_id) {
      spdlog::info("gpu thread wakes up");
      auto network_res = load_network(gpu_id);
      if (!network_res.ok()) {
         spdlog::error("could not load network: {}", network_res.msg());
         return;
      }
      auto network = network_res.unwrap();
      spdlog::info("thread {} emplaces network, nullptr{}=net", thread_name(), nullptr==network ? "=" : "!");
      g_networks.emplace(std::this_thread::get_id(), std::move(network));
      spdlog::info("thread {} emplaced network", thread_name());
      // std::this_thread::sleep_for(std::chrono::microseconds((int) 5e3));
      ++m_workers_up;
      m_cond->notify_one();
      spdlog::info("thread {} waked up", thread_name());
      while (!m_done) {
         util::DataWithPromise< std::vector<float>, std::tuple<std::span<float>, float> > np;
         if (work_queue.try_pop(np)) {
            try {
               if (!m_done) np.promise->set_value(just_forward(np.data_in));
            } catch (std::future_error& err) {
               spdlog::error("thread {} error: {}", thread_name(), err.what());
            }
         } else {
            std::this_thread::yield();
            std::this_thread::sleep_for(std::chrono::microseconds(1));
         }
      }
      m_threads_finished = true;
   }
   

 public:
   thread_pool(mb::size workers_per_gpu);
   thread_pool(const thread_pool&) = delete;
   thread_pool(thread_pool&&) = default;
   ~thread_pool() {
      m_done = true;
      while(!m_threads_finished) {
         spdlog::info("thread {} waiting for threads_finished", thread_name());
         std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }
      spdlog::info("thread {} clearing networks", thread_name());
      g_networks.clear();
      spdlog::info("thread {} cleared networks", thread_name());
   }
   void done() {
      m_done = true;
   }

   void submit(util::DataWithPromise< std::vector<float>, std::tuple<std::span<float>, float> > np) {
      work_queue.push(np);
   }
};

struct Context;

class client_threads {
   std::atomic_bool m_done;
   std::vector<std::thread> m_threads;
   join_threads m_joiner;
   std::unique_ptr<Context>& m_ctx_ptr;
   Player m_player;
   void client_work(unsigned cpu_id);

 public:
   client_threads(
      unsigned cpus_count, 
      std::unique_ptr<Context> &_ctx_ptr,
      Player _player )
       : m_done(false), m_joiner(m_threads), m_ctx_ptr(_ctx_ptr), m_player(_player)
   {
      if (cpus_count == 0)
         cpus_count = Eden_resources::get_cpus_count();
      spdlog::info("creating {} client threads", cpus_count);
      try {
         for (unsigned i = 0; i < cpus_count; ++i) {
            m_threads.push_back(std::thread(&client_threads::client_work, this, i));
         }
      } catch (...) {
         m_done = true;
         throw;
      }
   }
   ~client_threads() {
      m_done = true;
      spdlog::warn("client_threads destroyed!");
   }
   void join_clients() {
      m_joiner.join_earlier();
   }
};

template<typename T>
class DataWrapper {
 public:
   T m_data{};
   bool terminate{false};
   bool terminated{false};
   std::mutex m_mutex;
   DataWrapper() {}
   DataWrapper(T &_data) : m_data(_data) {}
   void notify(const std::shared_ptr<std::condition_variable> &_condVar) {
      _condVar->notify_one();
   }
};

struct MoveReadyness {
   bool dataReady{false};
   bool resultReady{false};
};

inline void await_for_data(
   const std::shared_ptr<DataWrapper<MoveReadyness>>& my_que,
   const std::shared_ptr<std::condition_variable>& condVar_computed)
{
   std::unique_lock<std::mutex> lck(my_que->m_mutex);
   spdlog::info("Client: Waiting...");
   condVar_computed->wait(lck, [&my_que] {
      spdlog::info("Client: resultReady={}",
                   (my_que->m_data.resultReady ? "true" : "false"));
      return my_que->m_data.resultReady;
   });
   spdlog::info("Client: dataReceived");
   my_que->m_data.resultReady = false;
}

inline void await_for_termination(
   const std::shared_ptr<DataWrapper<MoveReadyness>>& my_que,
   const std::shared_ptr<std::condition_variable>& condVar_computed)
{
   std::mutex mutex;
   std::unique_lock<std::mutex> lck(mutex);
   spdlog::info("Client: Waiting for termination...");
   condVar_computed->wait(lck, [&my_que] {
      spdlog::info("Client: terminate={}",
                   (my_que->terminated ? "true" : "false"));
      return my_que->terminated;
   });
   spdlog::info("Client: t1 terminated");
}

} // namespace::rl

#endif // CARCASSONNE_RL_CONCURRENCY_H
