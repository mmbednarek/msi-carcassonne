#include <Carcassonne/Training/Training.h>
#include <Carcassonne/RL/Concurrency.h>

namespace carcassonne::training {

Training::Training(uint64_t seed)
        : m_seed(seed),
          m_workers_pool(std::make_unique<ai::rl::thread_pool>(m_workers_per_gpu)),
          m_data_creator_pool(std::make_unique<ai::rl::data_creator_pool>(m_rl_count, m_workers_pool, m_trees_count)) {}

void Training::run() {
   std::vector<uint64_t> seeds{m_games_count};
   uint64_t last_seed = 0;
   while (m_running) {
      spdlog::warn("seed = {}", last_seed);
      std::iota(seeds.begin(), seeds.end(), last_seed);
      std::vector<std::promise<OneGame>> promises{m_games_count};
      std::vector<util::DataWithPromise<uint64_t, OneGame>> data{m_games_count};
      std::transform(seeds.begin(), seeds.end(), promises.begin(), data.begin(),
                     [](uint64_t &s, std::promise<OneGame> &og) { return util::DataWithPromise<uint64_t, OneGame>{&og, &s}; });
      for (int i = 0; i < m_games_count; ++i) {
         m_data_creator_pool->submit(data[i]);
      }
      for (int i = 0; i < m_games_count; ++i) {
         m_new_training_data.emplace_back(data[i].promise->get_future().get());
      }
      train_network();
      create_training_checkpoint();
      last_seed += m_games_count;
   }
}

void Training::train_network() {
   spdlog::debug("m_new_training_data[0].size={}", m_new_training_data[0].size());
   auto tid_net_map_it = ai::rl::g_networks.begin();
   tid_net_map_it->second->train(m_new_training_data);
}

}