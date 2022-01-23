#include <Carcassonne/RL/Concurrency.h>

namespace carcassonne::ai::rl {

std::map<std::thread::id, std::unique_ptr<Network>> g_networks;

mb::result<std::unique_ptr<Network>> load_network(int gpu_id) {
   caffe::Caffe::set_mode(caffe::Caffe::GPU);
   caffe::Caffe::SetDevice(gpu_id);
   spdlog::warn("load_network: device={}", gpu_id);

   caffe::SolverParameter solver_param;
   std::string param_file = std::string("./proto/solver.prototxt");
   fLI::FLAGS_minloglevel=3;
   caffe::ReadSolverParamsFromTextFileOrDie(param_file, &solver_param);
   return std::make_unique<Network>(std::move(solver_param), gpu_id);
}

thread_pool::thread_pool(mb::size workers_per_gpu)
 : m_done(false)
 , m_cond(std::make_unique<std::condition_variable>()) {
   unsigned gpus_count = Eden_resources::get_gpus_count();
   m_networks_count = gpus_count * workers_per_gpu;
   try {
      for (unsigned i = 0; i < m_networks_count; ++i) {
         unsigned gpu_id = i % gpus_count;
         m_threads.push_back(std::thread(&thread_pool::worker_thread, this, gpu_id));
         m_threads[i].detach();
         // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      }
      std::mutex mut;
      std::unique_lock<std::mutex> lck(mut);
      m_cond->wait(lck, [this] {
         spdlog::info("all_{}_networks_up={}", m_networks_count,
                        (m_workers_up == m_networks_count ? "true" : "false"));
         return m_workers_up == m_networks_count;
      });
   } catch (...) {
      m_done = true;
      throw;
   }
}

}