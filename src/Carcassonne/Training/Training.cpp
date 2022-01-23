#include <Carcassonne/Training/Training.h>
#include <Carcassonne/RL/Concurrency.h>
#include <Util/Time.h>

namespace carcassonne::training {

Training::Training(mb::u64 seed, std::mt19937& generator)
  : m_workers_per_gpu(1)
  , m_games_count(512)
  , m_training_set_size(100'000)
  , m_seed(seed)
  , m_promises(std::make_unique<std::vector<std::promise<OneGame>>>(m_games_count))
  , m_data(std::make_unique<std::vector<util::DataWithPromise<mb::u64, OneGame>>>(m_games_count))
  , m_rl_count(2)
  , m_generator(generator) {}
Training::~Training() {
   spdlog::warn("~Training exits");
}

void Training::run() {
   std::unique_ptr<ai::rl::thread_pool> workers_pool{std::make_unique<ai::rl::thread_pool>(m_workers_per_gpu)};
   std::unique_ptr<ai::rl::data_creator_pool> data_creator_pool{std::make_unique<ai::rl::data_creator_pool>(m_rl_count, workers_pool, m_trees_count, 512)};
   // std::this_thread::sleep_for(std::chrono::milliseconds(2000));
   spdlog::warn("run_started !!\n");
   auto start_run = util::unix_time();
   mb::u64 first_seed = 2000000;//m_seed;
   mb::u64 last_seed = first_seed;//m_seed;
   while (m_running) {
      spdlog::warn("seed = {}", last_seed);
      std::vector<mb::u64> seeds{m_games_count, 0};
      // std::iota(seeds.begin(), seeds.end(), last_seed);
      for (mb::u64 i = 0; i < m_games_count; ++i) {
         seeds[i] = i + last_seed;
      }
      for (int i = 0; i < m_games_count; ++i) {
         m_data->at(i) = util::DataWithPromise<mb::u64, OneGame>{
               .promise = &m_promises->at(i),
               .data_in = &seeds[i]
            };
      }
      spdlog::warn("Generating started!");
      for (int i = 0; i < m_games_count; ++i) {
         // std::this_thread::sleep_for(std::chrono::milliseconds(123));
         data_creator_pool->submit(m_data->at(i));
      }
      for (int i = 0; i < m_games_count; ++i) {
         // std::this_thread::sleep_for(std::chrono::milliseconds(10));
         m_full_training_data.emplace_back(m_data->at(i).promise->get_future().get());
         // m_data->at(i).promise->get_future().get();
         spdlog::warn("Seed {}={} finished !!!", *m_data->at(i).data_in, seeds[i]);
      }
      spdlog::warn("Generating finished !!!");
      spdlog::warn("Clamp trainingset to size {}", m_training_set_size);
      if (m_full_training_data.size() > m_training_set_size) {
         mb::u64 to_delete = m_full_training_data.size() - m_training_set_size;
         std::rotate(m_full_training_data.begin(), m_full_training_data.begin()+to_delete, m_full_training_data.end());
         m_full_training_data.resize(m_training_set_size);
         spdlog::warn("Training: training_data.size {}", m_full_training_data.size() );
      }

      spdlog::warn("Sampling batch of size {}", ai::rl::g_batch_size);
      std::array<double,4> intervals {0.0, 0.40, 0.50, 1.0};
      for (auto &interval : intervals) interval *= static_cast<double>(m_full_training_data.size());
      std::array<double,4> weights {100.0, 75.0, 25.0, 0.0};
      auto sampled_training_data = util::choose_n_picewise<OneGame, ai::rl::g_batch_size>(m_generator, m_full_training_data, intervals, weights);
      spdlog::warn("Sampled batch");
      train_network(sampled_training_data);
      last_seed += m_games_count;
      spdlog::warn("last_seed={}\n", last_seed);
      spdlog::warn("Training: run lasted {}ms", util::unix_time() - start_run);
      spdlog::warn("run_finished {}-{}{}{}?\n", last_seed, first_seed, last_seed - first_seed == m_games_count ? "==" : "!=", m_games_count);
      if (last_seed - first_seed == m_games_count) {
         spdlog::warn("run_finished !!\n");
         m_running = false;
      }
   }
   data_creator_pool->done();
   workers_pool->done();
}

void Training::train_network(const std::array<OneGame*, ai::rl::g_batch_size>& batch) {
   auto tid_net_map_it = ai::rl::g_networks.begin();
   tid_net_map_it->second->train(batch);
}

}