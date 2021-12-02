#include <Carcassonne/AI/HeuristicPlayer.h>
#include <Carcassonne/AI/MCTSPlayer.h>
#include <Carcassonne/AI/RandomPlayer.h>
#include <Carcassonne/Frontend/GameView.h>
#include <Carcassonne/Frontend/Resource.h>
#include <Carcassonne/Game/Game.h>
#include <Graphics/Surface.h>
#include <Input/Input.h>
#include <chrono>
#include <fmt/core.h>
#include <mb/core.h>

uint64_t now_milis() {
   return std::chrono::duration_cast<std::chrono::milliseconds>(
                  std::chrono::system_clock::now().time_since_epoch())
           .count();
}

constexpr bool player_range_ok(int count) {
   return count >= 0 && count <= 4;
}

#include <caffe/caffe.hpp>
#include <caffe/filler.hpp>
#include <caffe/util/math_functions.hpp>
#include <caffe/net.hpp>
#include <caffe/layers/memory_data_layer.hpp>
#include <caffe/layers/input_layer.hpp>
#include <caffe/sgd_solvers.hpp>

namespace caffe {

void test_caffe1() {
  SyncedMemory data_a(10 * sizeof(unsigned int));
  Caffe::set_random_seed(1701);
  caffe_rng_bernoulli(10, 0.5, static_cast<int*>(data_a.mutable_cpu_data()));
  for (int i = 0; i < 10; ++i) {
    printf("%d ", ((const unsigned int *)(data_a.cpu_data()))[i]);
  }
  printf("\n");
}

}

void test_net() {
   fmt::print("Passing the input to it for training\n");
   float *data = new float[64*1*1*3*400];
   float *label = new float[64*1*1*1*400];
   for(int i = 0; i<64*1*1*400; ++i)
   {
      int a = rand() % 2;
      int b = rand() % 2;
      int c = a ^ b;
      data[i*2 + 0] = a;
      data[i*2 + 1] = b;
      label[i] = c;
   }

   if (caffe::Caffe::mode() == caffe::Caffe::CPU) printf("mode=CPU\n");
   if (caffe::Caffe::mode() == caffe::Caffe::GPU) printf("mode=GPU\n");
   caffe::Caffe::set_mode(caffe::Caffe::CPU);
   if (caffe::Caffe::mode() == caffe::Caffe::CPU) printf("mode=CPU\n");
   if (caffe::Caffe::mode() == caffe::Caffe::GPU) printf("mode=GPU\n");

   fmt::print("Training preparation\n");
   caffe::SolverParameter solver_param;
   caffe::ReadSolverParamsFromTextFileOrDie("./solver.prototxt", &solver_param);
//   std::shared_ptr<caffe::Solver<float>> solver(
//       caffe::SolverRegistry<float>::CreateSolver(solver_param));
  caffe::SGDSolver<float>* solver = new caffe::SGDSolver<float>(solver_param);
   caffe::MemoryDataLayer<float> *dataLayer_trainnet =
       (caffe::MemoryDataLayer<float>
            *)(solver->net()->layer_by_name("inputdata").get());

   dataLayer_trainnet->Reset(data, label, 25600);

   fmt::print("Training: ");
   for (int i = 9; i >= 0; --i) {
      fmt::print("{} ", i);
      fflush(stdout);
      solver->Step(500000);
      // solver->net()->blobs()->data()
      // fmt::print("loss={} ", *(solver->losses_.end() - 1));
   }
   fmt::print("\n");

   fmt::print("Testing preparation\n");
   std::shared_ptr<caffe::Net<float>> testnet;
   testnet.reset(new caffe::Net<float>("./model.prototxt", caffe::TEST) );
   testnet->CopyTrainedLayersFrom("XOR_iter_5000000.caffemodel");

   fmt::print("Passing the input to it for testing\n");
   float testab[] = {0, 0, 0, 1, 1, 0, 1, 1};
   float testc[] = {0, 1, 1, 0};
   caffe::MemoryDataLayer<float> *dataLayer_testnet =
       (caffe::MemoryDataLayer<float>
            *)(testnet->layer_by_name("test_inputdata").get());
   dataLayer_testnet->Reset(testab, testc, 4);
   
   fmt::print("Testing\n");
   testnet->Forward();

   boost::shared_ptr<caffe::Blob<float>> output_layer =
       testnet->blob_by_name("output");
   const float *begin = output_layer->cpu_data();
   const float *end = begin + 4;

   fmt::print("Move data to a vector\n");
   std::vector<float> result(begin, end);
   for (int i = 0; i < result.size(); ++i) {
     printf("input: %d xor %d,  truth: %f result by nn: %f\n",
            (int)testab[i * 2 + 0], (int)testab[i * 2 + 1], testc[i],
            result[i]);
   }
   free(data); free(label);
}

int main() {
   google::InitGoogleLogging("./out/log.txt");
   caffe::test_caffe1();
   test_net();

   using carcassonne::frontend::Status;

   auto game_seed = std::stoull(mb::getenv("C_SEED").unwrap("9"));
   auto human_player_count = std::stoi(mb::getenv("C_HUMAN_PLAYERS").unwrap("1"));
   auto random_ai_player_count = std::stoi(mb::getenv("C_AI_RANDOM_PLAYERS").unwrap("0"));
   auto mcts_ai_player_count = std::stoi(mb::getenv("C_AI_MCTS_PLAYERS").unwrap("1"));
   auto heuristic_ai_player_count = std::stoi(mb::getenv("C_AI_HEURISTIC_PLAYERS").unwrap("0"));

   if (!player_range_ok(human_player_count)) {
      fmt::print(stderr, "invalid human player count (0-4): {}", human_player_count);
      return 1;
   }

   if (!player_range_ok(random_ai_player_count)) {
      fmt::print(stderr, "invalid random ai player count (0-4): {}", random_ai_player_count);
      return 1;
   }

   if (!player_range_ok(mcts_ai_player_count)) {
      fmt::print(stderr, "invalid random ai player count (0-4): {}", random_ai_player_count);
      return 1;
   }

   if (!player_range_ok(human_player_count + random_ai_player_count + mcts_ai_player_count + heuristic_ai_player_count)) {
      fmt::print(stderr, "invalid player count (0-4): {}", human_player_count + random_ai_player_count + mcts_ai_player_count + heuristic_ai_player_count);
      return 1;
   }

   graphics::Config cfg{
           .width = std::stoi(mb::getenv("WND_WIDTH").unwrap("800")),
           .height = std::stoi(mb::getenv("WND_HEIGHT").unwrap("600")),
           .title = "Carcassonne",
   };
   auto surface_re = graphics::Surface::create(cfg);
   if (!surface_re.ok()) {
      fmt::print(stderr, "could not initialise graphic API: {}", surface_re.msg());
      return 1;
   }
   auto surface = surface_re.unwrap();

   auto resource_re = carcassonne::frontend::ResourceManager::the().load_resources(surface);
   if (!resource_re.ok()) {
      fmt::print(stderr, "could not load all resources: {}", resource_re.msg());
      return 1;
   }

   carcassonne::game::Game game(human_player_count + random_ai_player_count + heuristic_ai_player_count + mcts_ai_player_count, game_seed);

   auto human_players = std::accumulate(
           carcassonne::g_players.begin(),
           carcassonne::g_players.begin() + human_player_count,
           carcassonne::PlayerAssignment::None,
           [](carcassonne::PlayerAssignment players, carcassonne::Player p) {
              return players | carcassonne::player_to_assignment(p);
           });
   carcassonne::frontend::GameView view(game, human_players);

   std::random_device random_device;
   std::mt19937 random_generator(random_device());

   std::vector<carcassonne::ai::RandomPlayer<>> random_players;
   random_players.reserve(random_ai_player_count);
   std::transform(carcassonne::g_players.begin() + human_player_count, carcassonne::g_players.begin() + (human_player_count + random_ai_player_count), std::back_inserter(random_players), [&random_generator](carcassonne::Player p) {
      return carcassonne::ai::RandomPlayer<>(random_generator, p);
   });
   for (auto &player : random_players) {
      player.await_turn(game);
   }

   std::vector<carcassonne::ai::HeuristicPlayer> heuristic_players;
   heuristic_players.reserve(heuristic_ai_player_count);
   std::transform(carcassonne::g_players.begin() + (human_player_count + random_ai_player_count), carcassonne::g_players.begin() + (human_player_count + random_ai_player_count + heuristic_ai_player_count), std::back_inserter(heuristic_players), [](carcassonne::Player p) {
      return carcassonne::ai::HeuristicPlayer(p);
   });
   for (auto &player : heuristic_players) {
      player.await_turn(game);
   }

   std::vector<std::unique_ptr<carcassonne::ai::MCTSPlayer>> mcts_players(mcts_ai_player_count);
   std::transform(carcassonne::g_players.begin() + (human_player_count + random_ai_player_count + heuristic_ai_player_count), carcassonne::g_players.begin() + (human_player_count + random_ai_player_count + heuristic_ai_player_count + mcts_ai_player_count), mcts_players.begin(), [&game](carcassonne::Player p) {
      return std::make_unique<carcassonne::ai::MCTSPlayer>(game, p, carcassonne::ai::SimulationType::Heuristic);
   });

   constexpr double dt = 1000.0 / 60.0;
   auto prev_time = static_cast<double>(now_milis());
   auto dt_accum = 0.0;

   game.start();

   while (view.status() != Status::Quitting) {
      auto now = static_cast<double>(now_milis());
      auto diff = now - prev_time;
      prev_time = now;

      dt_accum += diff;
      while (dt_accum > dt) {
         dt_accum -= dt;
         game.update(dt);
         view.update(dt);
      }
      carcassonne::frontend::ResourceManager::the().pre_render_hook(surface);
      view.render(surface.context());
      carcassonne::frontend::ResourceManager::the().post_render_hook();
      input::handle_events(view);
   }
   return 0;
}
