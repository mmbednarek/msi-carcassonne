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
   // caffe::NetParameter net_parameter;
   // std::string model_file = std::string("./proto/net_full_alphazero_40_res_blocks.prototxt");
   // std::ifstream t(model_file.c_str());
   // std::string model((std::istreambuf_iterator<char>(t)),
   //                   std::istreambuf_iterator<char>());
   // bool success = google::protobuf::TextFormat::ParseFromString(model, &net_parameter);
   // if (!success) {
   //    return mb::error("could not parse protobuf file");
   // }
   // net_parameter.mutable_state()->set_phase(caffe::TRAIN);
   return std::make_unique<Network>(solver_param, gpu_id);
}

}