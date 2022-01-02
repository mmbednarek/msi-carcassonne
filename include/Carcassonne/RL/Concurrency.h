#ifndef CARCASSONNE_RL_CONCURRENCY_H
#define CARCASSONNE_RL_CONCURRENCY_H
#include <Carcassonne/RL/Network.h>
#include <Eden_resources/Ngpus_Ncpus.h>
#include <Carcassonne/AI/DeepRL.h>
#include <Carcassonne/AI/Tree.h>
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

inline static std::map<std::thread::id, std::unique_ptr<Network>> g_networks;
inline static std::map<std::thread::id, std::unique_ptr<Tree>> g_trees;

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

 public:
   explicit join_threads(std::vector<std::thread> &threads_) : threads(threads_) {}
   ~join_threads() {
      for (unsigned long i = 0; i < threads.size(); ++i) {
         if (threads[i].joinable())
            threads[i].join();
      }
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

mb::result<std::unique_ptr<carcassonne::ai::rl::Network>> DeepRLPlayer::load_network(int gpu_id) const {
   caffe::Caffe::set_mode(caffe::Caffe::GPU);
   caffe::Caffe::SetDevice(gpu_id);
   
   caffe::SolverParameter solver_param;
   caffe::ReadSolverParamsFromTextFileOrDie("./proto/solver.prototxt", &solver_param);
   
   caffe::NetParameter net_parameter;
   std::ifstream t("./proto/net_full_alphazero_40_res_blocks.prototxt");
   std::string model((std::istreambuf_iterator<char>(t)),
                     std::istreambuf_iterator<char>());
   bool success = google::protobuf::TextFormat::ParseFromString(model, &net_parameter);
   if (!success) {
      return mb::error("could not parse protobuf file");
   }
   
   net_parameter.mutable_state()->set_phase(caffe::TRAIN);
   
   return std::make_unique<carcassonne::ai::rl::Network>(net_parameter, solver_param);
}

class thread_pool {
   std::atomic_bool done;
   std::vector<std::thread> threads;
   join_threads joiner;
   threadsafe_queue<function_wrapper> work_queue;
   void worker_thread(int gpu_id) {
      auto network_res = load_network(gpu_id);
      if (!network_res.ok()) {
         spdlog::error("could not load network: {}", network_res.msg());
         return EXIT_FAILURE;
      }
      auto network = network_res.unwrap();
      g_networks.emplace(std::this_thread::get_id(), std::move(load_network(gpu_id)));
      std::cout << "worker Thread " << std::this_thread::get_id() << " run on gpu " << gpu_id << std::endl;
      std::this_thread::sleep_for(std::chrono::microseconds((int) 5e3));
      while (!done) {
         function_wrapper task;
         if (work_queue.try_pop(task)) {
            task();
         } else {
            std::this_thread::yield();
            std::this_thread::sleep_for(std::chrono::microseconds(1));
         }
      }
   }

 public:
   thread_pool() : done(false), joiner(threads) {
      // unsigned const thread_count = std::thread::hardware_concurrency();
      unsigned gpus_count = Eden_resources::get_gpus_count();
      try {
         for (unsigned i = 0; i < gpus_count; ++i) {
            threads.push_back(std::thread(&thread_pool::worker_thread, this, i));
         }
      } catch (...) {
         done = true;
         throw;
      }
   }
   ~thread_pool() {
      done = true;
   }

   template<typename FunctionType>
   std::future<typename std::result_of<FunctionType()>::type>
   submit(FunctionType f) {
      typedef typename std::result_of<FunctionType()>::type
              result_type;
      std::packaged_task<result_type()> task(std::move(f));
      std::future<result_type> res(task.get_future());
      work_queue.push(std::move(task));
      return res;
   }
};

class client_threads {
   std::atomic_bool done;
   std::vector<std::thread> threads;
   join_threads joiner;
   std::mutex mx_cout;
   std::unique_ptr<rl::Context> ctx_ptr;
   void client_work(unsigned cpu_id);

 public:
   client_threads(std::unique_ptr<rl::Context>&& _ctx_ptr)
    : done(false)
    , joiner(threads)
    , ctx_ptr(std::move(_ctx_ptr))
   {
      unsigned const cpus_count = Eden_resources::get_cpus_count();
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
};

} // namespace::rl

#endif // CARCASSONNE_RL_CONCURRENCY_H