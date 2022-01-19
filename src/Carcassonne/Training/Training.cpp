#include <Carcassonne/Training/Training.h>
#include <Carcassonne/RL/Concurrency.h>

namespace carcassonne::training {

Training::Training(uint64_t seed)
        : m_games_count(5)
        , m_seed(seed)
        , m_workers_pool(std::make_unique<ai::rl::thread_pool>(m_workers_per_gpu))
        , m_data_creator_pool(std::make_unique<ai::rl::data_creator_pool>(m_rl_count, m_workers_pool, m_trees_count)) {}
Training::~Training() {
   m_workers_pool->done();
}

void Training::run() {
   spdlog::debug("run_started !!=\n");
   auto start_run = util::unix_time();
   uint64_t last_seed = 1000;//m_seed;
   while (m_running) {
      std::vector<uint64_t> seeds{m_games_count, 0};
      std::vector<std::promise<OneGame>> promises{m_games_count};
      std::vector<util::DataWithPromise<uint64_t, OneGame>> data;
      spdlog::warn("seed = {}", last_seed);
      // std::iota(seeds.begin(), seeds.end(), last_seed);
      for (uint64_t i = 0; i < m_games_count; ++i) {
         seeds[i] = i + last_seed;
      }
      // for (int i = 0; i < m_games_count; ++i) {
      //    data[i] = util::DataWithPromise<uint64_t, OneGame>{
      //          .promise = &promises[i],
      //          .data_in = &seeds[i]
      //       };
      // }
      for (int i = 0; i < m_games_count; ++i) {
         data.emplace_back(promises[i], seeds[i]);
      }
      // for (int i = 0; i < m_games_count; ++i) {
      //    data.push_back(util::DataWithPromise<uint64_t, OneGame>{
      //          .promise = &promises[i],
      //          .data_in = &seeds[i]
      //       } );
      // }
      // std::transform(seeds.begin(), seeds.end(), promises.begin(), data.begin(),
      //                [](uint64_t &s, std::promise<OneGame> &og) { fmt::print("seed_og={}", s); return util::DataWithPromise<uint64_t, OneGame>{&og, &s}; });
      for (int i = 0; i < m_games_count; ++i) {
         std::this_thread::sleep_for(std::chrono::milliseconds(500));
         m_data_creator_pool->submit(data[i]);
      }
      for (int i = 0; i < m_games_count; ++i) {
         std::this_thread::sleep_for(std::chrono::milliseconds(10));
         m_new_training_data.emplace_back(data[i].promise->get_future().get());
         // data[i].promise->get_future().get();
         spdlog::warn("Seed {}={} finished !!!", *data[i].data_in, seeds[i]);
      }
      spdlog::warn("Generating finished !!!");
      train_network();
      last_seed += m_games_count;
      spdlog::debug("last_seed={}\n", last_seed);
      spdlog::debug("Training: run lasted {}ms", util::unix_time() - start_run);
      spdlog::debug("run_finished !!=\n");
   }
}

void Training::train_network() {
   auto tid_net_map_it = ai::rl::g_networks.begin();
   tid_net_map_it->second->train(m_new_training_data);
}

}