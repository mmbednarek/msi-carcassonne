#ifndef CARCASSONNE_RL_CONCURRENCY_H
#define CARCASSONNE_RL_CONCURRENCY_H
#pragma once
#include <Carcassonne/RL/ThreadSafeQueue.h>
#include <Carcassonne/RL/Network.h>
#include <Carcassonne/AI/Tree.h>
#include <Eden_resources/Ngpus_Ncpus.h>
#include <Util/DataWithPromise.h>
#include <google/protobuf/text_format.h>
#include <Util/Time.h>
#include <spdlog/spdlog.h>
#include <chrono>
#include <future>
#include <thread>
#include <utility>
#include <vector>
#include <span>
#include <tuple>
#include <map>
#include <functional>
#include <random>
#include <iostream>

namespace carcassonne::ai::rl {

extern std::map<std::thread::id, std::unique_ptr<Network>> g_networks;

// static FullMove get_move(const std::unique_ptr<IGame> &game) {
// #ifdef MEASURE_TIME
//    fmt::print("\n{}\n", g_max_moves - game->move_index());
// #endif
//    std::hash<std::thread::id> hasher;
//    unsigned tid = hasher(std::this_thread::get_id());
//    spdlog::debug("thread {} acquires network", thread_name());
//    std::unique_ptr<Network> &net_ptr = g_networks[std::this_thread::get_id()];
//    spdlog::debug("thread {} acquired network", thread_name());
//    FullMove mv = net_ptr->do_move(game, static_cast<float>(rand_r(&tid) % 1000) / 1000.0f);// rand is a hash
//    if (mv.ignored_figure) {
//       if (!game->board().can_place_at(mv.x, mv.y, game->tile_set()[game->move_index()], mv.rotation)) {
//       }
//    } else if (!game->can_place_tile_and_figure(mv.x, mv.y, mv.rotation, game->tile_set()[game->move_index()], mv.direction)) {
//    }
//    return mv;
// }

// inline Player simulate(Node* node) {
//    auto start = util::unix_time();
//    auto simulated_game = node->game().clone();
//    for (auto move_index = simulated_game->move_index(); move_index < g_max_moves; ++move_index) {
// #ifdef MEASURE_TIME
//       auto start_move = util::unix_time();
// #endif
//       auto current_player = simulated_game->current_player();
//       auto full_move = get_move(simulated_game);
//       auto move = simulated_game->new_move(current_player);
//       move->place_tile_at(full_move.x, full_move.y, full_move.rotation);
//       move->place_figure(full_move.direction);
//       simulated_game->update(0);
// #ifdef MEASURE_TIME
//       spdlog::debug("deep rl: move lasted {}ms", util::unix_time() - start_move);
// #endif
//    }
//    auto max_score_it = std::max_element(simulated_game->scores().begin(), simulated_game->scores().end(), [](PlayerScore lhs, PlayerScore rhs) {
//       return lhs.score < rhs.score;
//    });
//    spdlog::debug("deep rl: simulation lasted {}ms", util::unix_time() - start);
//    return max_score_it->player;
// }

mb::result<std::unique_ptr<Network>> load_network(int gpu_id);

inline std::tuple<std::span<float>, float> just_forward(std::vector<float> *data_in) {
   auto &neuron_inputs = data_in;
   std::unique_ptr<Network> &network = g_networks[std::this_thread::get_id()];
   boost::shared_ptr<caffe::Blob<float>> input_data{network->solver().net()->blob_by_name("input_data")};
   boost::shared_ptr<caffe::Blob<float>> output_probas{network->solver().net()->blob_by_name("output_probas")};
   boost::shared_ptr<caffe::Blob<float>> output_value{network->solver().net()->blob_by_name("output_value")};
   std::copy(neuron_inputs->begin(), neuron_inputs->end(), input_data->mutable_cpu_data());
   network->solver().net()->Forward();
   static constexpr auto output_neuron_count =  g_board_width * g_board_height * 4 * 10;
   // filtering moves:
   // std::span<float> out_span(output_probas->mutable_cpu_data(), output_neuron_count);
   // std::vector<std::reference_wrapper<float>> out_refs(output_probas->mutable_cpu_data(), output_probas->mutable_cpu_data() + output_neuron_count);
   // std::vector<bool> allowed_moves;
   // auto sum_neurons = decoder::fill_allowed_vec(&game, allowed_moves, out_span);
   // out_refs.erase(std::remove_if(out_refs.begin(), out_refs.end(), [&allowed_moves, &out_refs, &sum_neurons](float& prob) { 
   //    bool ok = allowed_moves[&prob - &(*out_refs.begin()).get()]; 
   //    if (ok) prob /= sum_neurons;
   //    return !ok;}), out_refs.end());
   return std::make_tuple(std::span<float>(output_probas->mutable_cpu_data(), output_neuron_count), *output_value->mutable_cpu_data());
}

class thread_pool {
   bool done;
   bool threads_finished = false;
   std::vector<std::thread> threads;
   join_threads joiner;
   threadsafe_queue<util::DataWithPromise< std::vector<float>, std::tuple<std::span<float>, float> >> work_queue;
   void worker_thread(int gpu_id) {
      spdlog::debug("thread {} wakes up", thread_name());
      auto network_res = load_network(gpu_id);
      // lck.lock();
      if (!network_res.ok()) {
         spdlog::error("could not load network: {}", network_res.msg());
         return;
      }
      auto network = network_res.unwrap();
      spdlog::debug("thread {} emplaces network, nullptr{}=net", thread_name(), nullptr==network ? "=" : "!");
      g_networks.emplace(std::this_thread::get_id(), std::move(network));
      spdlog::debug("thread {} emplaced network", thread_name());
      // lck.unlock();
      // std::cout << "worker Thread " << std::this_thread::get_id() << " run on gpu " << gpu_id << std::endl;
      std::this_thread::sleep_for(std::chrono::microseconds((int) 5e3));
      spdlog::debug("thread {} waked up", thread_name());
      while (!done) {
         util::DataWithPromise< std::vector<float>, std::tuple<std::span<float>, float> > np;
         if (work_queue.try_pop(np)) {
            try {
               if (!done) np.promise->set_value(just_forward(np.data_in));
            } catch (std::future_error& err) {
               spdlog::error("thread {} error: {}", thread_name(), err.what());
            }
         } else {
            std::this_thread::yield();
            std::this_thread::sleep_for(std::chrono::microseconds(1));
         }
      }
      threads_finished = true;
   }
   

 public:
   thread_pool(mb::size workers_per_gpu) : done(false), joiner(threads) {
      // unsigned const thread_count = std::thread::hardware_concurrency();
      unsigned gpus_count = Eden_resources::get_gpus_count();
      // gpus_count = 1;
      unsigned networks_count = gpus_count * workers_per_gpu;
      try {
         for (unsigned i = 0; i < networks_count; ++i) {
            unsigned gpu_id = i % gpus_count;
            std::this_thread::sleep_for(std::chrono::seconds(2));
            threads.push_back(std::thread(&thread_pool::worker_thread, this, gpu_id));
            std::this_thread::sleep_for(std::chrono::seconds(2));
         }
         std::this_thread::sleep_for(std::chrono::seconds(5));
      } catch (...) {
         done = true;
         throw;
      }
   }
   thread_pool(const thread_pool&) = delete;
   thread_pool(thread_pool&&) = default;
   ~thread_pool() {
      done = true;
      while(!threads_finished) {
         spdlog::debug("thread {} waiting for threads_finished", thread_name());
         std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }
      spdlog::debug("thread {} clearing networks", thread_name());
      g_networks.clear();
      spdlog::debug("thread {} cleared networks", thread_name());
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
      spdlog::debug("creating {} client threads", cpus_count);
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
