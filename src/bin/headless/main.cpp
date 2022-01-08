#include <Carcassonne/AI/DeepRLPlayer.h>
#include <Carcassonne/AI/RandomPlayer.h>
#include <Carcassonne/Encoder/Rollout.h>
#include <Carcassonne/Game/Game.h>
#include <Carcassonne/Training/Gameplay.h>
#include <boost/program_options.hpp>
#include <google/protobuf/text_format.h>
#define SPDLOG_FMT_EXTERNAL
#include <spdlog/spdlog.h>

namespace po = boost::program_options;

int main(int argc, char **argv) {
   po::options_description desc("carcassonne headless");
   desc.add_options()
   ("help", "")
   ("seed,s", po::value<int>()->default_value(10), "seed of a game")
   ("rl-ai-count,l", po::value<int>()->default_value(1), "number of rl agents")
   ("random-ai-count,r", po::value<int>()->default_value(1), "number of random agents")
   ("verbose,v", "verbose output");
   unsigned trees_count = 1;
   unsigned workers_per_gpu = 1;

   po::variables_map values;
   po::store(po::command_line_parser(argc, argv).options(desc).run(), values);

   auto seed = values["seed"].as<int>();
   auto rl_count = values["rl-ai-count"].as<int>();
   auto random_count = values["random-ai-count"].as<int>();

   if (values.contains("verbose")) {
      spdlog::set_level(spdlog::level::debug);
   }

   auto total_player_count = rl_count + random_count;
   if (total_player_count > 4) {
      spdlog::error("player count may not exceed 4");
      return EXIT_FAILURE;
   }

   std::promise<carcassonne::training::OneGame> p;
   carcassonne::training::Gameplay gameplay(total_player_count, seed, trees_count, &p);
   std::mt19937 generator(seed);
   std::unique_ptr<carcassonne::ai::rl::thread_pool> workers_pool = 
      std::make_unique<carcassonne::ai::rl::thread_pool>(workers_per_gpu);

   for (int i = 0; i < rl_count; ++i) {
      gameplay.add_rl_player(generator, workers_pool);
   }

   for (int i = 0; i < random_count; ++i) {
      gameplay.add_random_player(generator);
   }

   gameplay.run();
   gameplay.save("gameplay.proto");
   spdlog::error("p.get_future().get().size()={}", p.get_future().get().size());
}