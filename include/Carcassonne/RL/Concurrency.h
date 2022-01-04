#ifndef CARCASSONNE_RL_CONCURRENCY_H
#define CARCASSONNE_RL_CONCURRENCY_H
#pragma once
#include <Carcassonne/RL/Network.h>
#include <Eden_resources/Ngpus_Ncpus.h>
#include <Carcassonne/AI/Tree.h>
#include <google/protobuf/text_format.h>
#include <Util/Time.h>
#include <spdlog/spdlog.h>
#include <chrono>
#include <condition_variable>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include <vector>
#include <map>
#include <functional>
#include <random>
#include <iostream>

namespace carcassonne::ai::rl {

extern std::map<std::thread::id, std::unique_ptr<Network>> g_networks;

template<typename T>
class threadsafe_queue {
 private:
   mutable std::mutex mut;
   std::queue<T> data_queue;
   std::condition_variable data_cond;

 public:
   threadsafe_queue() {}
   threadsafe_queue(threadsafe_queue const &other) {
      std::lock_guard<std::mutex> lk(other.mut);
      data_queue = other.data_queue;
   }
   void push(T new_value) {
      std::lock_guard<std::mutex> lk(mut);
      data_queue.emplace(std::move(new_value));
      data_cond.notify_one();
   }
   void wait_and_pop(T &value) {
      std::unique_lock<std::mutex> lk(mut);
      data_cond.wait(lk, [this] { return !data_queue.empty(); });
      value = data_queue.front();
      data_queue.pop();
   }
   std::shared_ptr<T> wait_and_pop() {
      std::unique_lock<std::mutex> lk(mut);
      data_cond.wait(lk, [this] { return !data_queue.empty(); });
      std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
      data_queue.pop();
      return res;
   }
   bool try_pop(T &value) {
      std::lock_guard<std::mutex> lk(mut);
      if (data_queue.empty())
         return false;
      // std::cout << "id: " << std::this_thread::get_id() << std::endl;
      value = std::move(data_queue.front());
      data_queue.pop();
      return true;
   }
   std::shared_ptr<T> try_pop() {
      std::lock_guard<std::mutex> lk(mut);
      if (data_queue.empty())
         return std::shared_ptr<T>();
      std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
      data_queue.pop();
      return res;
   }
   bool empty() const {
      std::lock_guard<std::mutex> lk(mut);
      return data_queue.empty();
   }
};

class join_threads {
   std::vector<std::thread> &threads;
   bool joined_earlier = false;
   void join_all() {
      for (unsigned long i = 0; i < threads.size(); ++i) {
         // if (threads[i].joinable())
         threads[i].join();
      }
   }
 public:
   explicit join_threads(std::vector<std::thread> &threads_) : threads(threads_) {}
   ~join_threads() {
      if (joined_earlier) return;
      join_all();
   }
   void join_earlier() {
      joined_earlier = true;
      join_all();
   }
};

class function_wrapper {
   struct impl_base {
      virtual void call() = 0;
      virtual ~impl_base() {}
   };
   std::unique_ptr<impl_base> impl;
   template<typename F>
   struct impl_type : impl_base {
      F f;
      impl_type(F &&f_) : f(std::move(f_)) {}
      void call() { f(); }
   };

 public:
   template<typename F>
   function_wrapper(F &&f) : impl(new impl_type<F>(std::move(f))) {}
   void operator()() { impl->call(); }
   function_wrapper() = default;
   function_wrapper(function_wrapper &&other) : impl(std::move(other.impl)) {}
   function_wrapper &operator=(function_wrapper &&other) {
      impl = std::move(other.impl);
      return *this;
   }
   function_wrapper(const function_wrapper &) = delete;
   function_wrapper(function_wrapper &) = delete;
   function_wrapper &operator=(const function_wrapper &) = delete;
};

struct NodeWithPromise {
   std::promise<Player> *promise{nullptr};
   Node *node{nullptr};

 public:
   NodeWithPromise() = default;
   NodeWithPromise(std::promise<Player> *_promise, Node *_node) : promise(_promise), node(_node) {}
};

static FullMove get_move(const std::unique_ptr<IGame> &game) {
#ifdef MEASURE_TIME
   fmt::print("\n{}\n", g_max_moves - game->move_index());
#endif
   std::hash<std::thread::id> hasher;
   unsigned tid = hasher(std::this_thread::get_id());
   spdlog::debug("thread {} acquires network", thread_name());
   std::unique_ptr<Network> &net_ptr = g_networks[std::this_thread::get_id()];
   spdlog::debug("thread {} acquired network", thread_name());
   FullMove mv = net_ptr->do_move(game, static_cast<float>(rand_r(&tid) % 1000) / 1000.0f, thread_name());// rand is a hash
   if (mv.ignored_figure) {
      if (!game->board().can_place_at(mv.x, mv.y, game->tile_set()[game->move_index()], mv.rotation)) {
      }
   } else if (!game->can_place_tile_and_figure(mv.x, mv.y, mv.rotation, game->tile_set()[game->move_index()], mv.direction)) {
   }
   return mv;
}

inline Player simulate(Node* node) {
   // std::hash<std::thread::id> hasher;
   // unsigned tid = hasher(std::this_thread::get_id());
   // spdlog::debug("thread {}: simulate ok0", thread_name());

   auto start = util::unix_time();
   // if(nullptr == node) spdlog::debug("thread {}: simulate nullptr == node", thread_name());
   // spdlog::debug("thread {}: simulate ok2", thread_name());

   auto simulated_game = node->game().clone();
   // if(nullptr == simulated_game) spdlog::debug("thread {}: simulate nullptr == simulated_game", thread_name());
   // spdlog::debug("thread {}: simulate ok1", thread_name());
   for (auto move_index = simulated_game->move_index(); move_index < g_max_moves; ++move_index) {
   // spdlog::debug("thread {}: simulate ok3", thread_name());
#ifdef MEASURE_TIME
      auto start_move = util::unix_time();
#endif
      auto current_player = simulated_game->current_player();
      auto full_move = get_move(simulated_game);
      auto move = simulated_game->new_move(current_player);
      move->place_tile_at(full_move.x, full_move.y, full_move.rotation);
      move->place_figure(full_move.direction);
      simulated_game->update(0);
#ifdef MEASURE_TIME
      spdlog::debug("deep rl: move lasted {}ms", util::unix_time() - start_move);
#endif
   }
   auto max_score_it = std::max_element(simulated_game->scores().begin(), simulated_game->scores().end(), [](PlayerScore lhs, PlayerScore rhs) {
      return lhs.score < rhs.score;
   });
   node->mark_as_simulated();
   spdlog::debug("deep rl: simulation lasted {}ms", util::unix_time() - start);
   return max_score_it->player;
}

class thread_pool {
   std::atomic_bool done;
   std::vector<std::thread> threads;
   join_threads joiner;
   threadsafe_queue<NodeWithPromise> work_queue;
   std::mutex mut;
   std::unique_lock<std::mutex> lck;
   unsigned networks_per_gpu = 1;
   void worker_thread(int gpu_id) {
      spdlog::debug("thread {} wakes up", thread_name());
      spdlog::debug("thread {} ok0.0", thread_name());
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
         NodeWithPromise np;
         if (work_queue.try_pop(np)) {
            np.promise->set_value(simulate(np.node));
         } else {
            std::this_thread::yield();
            std::this_thread::sleep_for(std::chrono::microseconds(1));
         }
      }
   }
   
   mb::result<std::unique_ptr<Network>> load_network(int gpu_id) const {
      // spdlog::debug("thread ok0");
      spdlog::debug("thread {} ok1", thread_name());
      caffe::Caffe::set_mode(caffe::Caffe::GPU);
      // spdlog::debug("thread {} ok2", thread_name();
      // caffe::Caffe::SetDevice(gpu_id);
      // spdlog::debug("thread {} ok3", thread_name();
      spdlog::warn("load_network: device={}", gpu_id);
      
      caffe::SolverParameter solver_param;
      std::string param_file = std::string("./proto/solver") + std::to_string(gpu_id) + std::string(".prototxt");
      // spdlog::debug("thread {} ok4 param_file={} ", thread_name(), param_file);
      caffe::ReadSolverParamsFromTextFileOrDie(param_file, &solver_param);
      // spdlog::debug("thread {} ok5", thread_name();
      caffe::NetParameter net_parameter;
      std::string model_file = std::string("./proto/net_full_alphazero_40_res_blocks") + std::to_string(gpu_id) + std::string(".prototxt");
      std::ifstream t(model_file.c_str());
      // spdlog::debug("thread {} ok6", thread_name();
      std::string model((std::istreambuf_iterator<char>(t)),
                        std::istreambuf_iterator<char>());
      // spdlog::debug("thread {} ok7", thread_name();
      bool success = google::protobuf::TextFormat::ParseFromString(model, &net_parameter);
      // spdlog::debug("thread {} ok8", thread_name();
      if (!success) {
         return mb::error("could not parse protobuf file");
      }
      // spdlog::debug("thread {} ok9", thread_name();
      
      net_parameter.mutable_state()->set_phase(caffe::TRAIN);
      
      // spdlog::debug("thread {} ok10", thread_name();
      return std::make_unique<Network>(net_parameter, solver_param, gpu_id);
   }

 public:
   thread_pool() : done(false), joiner(threads), lck(mut, std::defer_lock) {
      // unsigned const thread_count = std::thread::hardware_concurrency();
      unsigned gpus_count = Eden_resources::get_gpus_count();
      // gpus_count = 1;
      unsigned networks_count = gpus_count * networks_per_gpu;
      try {
         for (unsigned i = 0; i < networks_count; ++i) {
            unsigned gpu_id = i % gpus_count;
      std::this_thread::sleep_for(std::chrono::seconds(5));
            threads.push_back(std::thread(&thread_pool::worker_thread, this, gpu_id));
      std::this_thread::sleep_for(std::chrono::seconds(5));
         }
      } catch (...) {
         done = true;
         throw;
      }
   }
   ~thread_pool() {
      done = true;
   }

   void submit(NodeWithPromise np) {
      work_queue.push(np);
   }
};

struct Context;

class client_threads {
   std::atomic_bool done;
   std::vector<std::thread> threads;
   join_threads joiner;
   std::unique_ptr<Context>& ctx_ptr;
   std::shared_ptr<std::condition_variable> cond_var;
   void client_work(unsigned cpu_id);

 public:
   client_threads(
      unsigned cpus_count, 
      std::unique_ptr<Context> &_ctx_ptr, 
      const std::shared_ptr<std::condition_variable>& _cond_var)
       : done(false), joiner(threads), ctx_ptr(_ctx_ptr), cond_var(_cond_var) {
      if (cpus_count == 0)
         cpus_count = Eden_resources::get_cpus_count();
      try {
         for (unsigned i = 0; i < cpus_count; ++i) {
            threads.push_back(std::thread(&client_threads::client_work, this, i));
         }
      } catch (...) {
         done = true;
         throw;
      }
   }
   ~client_threads() {
      done = true;
   }
   void join_clients() {
      joiner.join_earlier();
   }
};

} // namespace::rl

#endif // CARCASSONNE_RL_CONCURRENCY_H
