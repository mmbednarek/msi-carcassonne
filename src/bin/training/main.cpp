#include <Carcassonne/AI/DeepRLPlayer.h>
#include <Carcassonne/AI/RandomPlayer.h>
#include <Carcassonne/Encoder/Rollout.h>
#include <Carcassonne/Game/Game.h>
#include <Carcassonne/Training/Training.h>
#include <boost/program_options.hpp>
#define SPDLOG_FMT_EXTERNAL
#include <spdlog/spdlog.h>

namespace po = boost::program_options;

int main(int argc, char **argv) {
   po::options_description desc("carcassonne training");
   desc.add_options()
   ("help", "")
   ("seed,s", po::value<int>()->default_value(10), "seed of a training")
   ("verbose,v", "verbose output");

   po::variables_map values;
   po::store(po::command_line_parser(argc, argv).options(desc).run(), values);

   auto seed = values["seed"].as<int>();

   if (values.contains("verbose")) {
      spdlog::set_level(spdlog::level::debug);
   }

   auto net_res = carcassonne::ai::rl::load_network(0);
   if (!net_res.ok()) {
     spdlog::error("could not load network: {}", net_res.msg());
     return EXIT_FAILURE;
   }
   auto net = net_res.unwrap();

   carcassonne::training::Training training(seed, *net);

   training.run();
}