#include "Carcassonne/RL/Concurrency.h"
#include <Carcassonne/Decoder/Decoder.h>
#include <Carcassonne/RL/Network.h>
#include <Eden_resources/Ngpus_Ncpus.h>
#include <Util/Time.h>
#include <fmt/core.h>
#include <future>
#include <span>
#include <google/protobuf/text_format.h>
// #define SPDLOG_FMT_EXTERNAL
#include <spdlog/spdlog.h>
#include <pthread.h>

namespace carcassonne::ai::rl {

// template<typename T, typename... ARGS>
// constexpr boost::shared_ptr<T> make_shared(ARGS... args) {
//    return boost::shared_ptr<T>(new T(args...));
// }

Network::Network(
   caffe::SolverParameter &&solver_param,
   int gpu_id )
    : m_solver(std::make_unique<caffe::SGDSolver<float>>(std::move(solver_param)))
    , m_allowed_moves(g_board_width * g_board_height * 4 * 14, false)
    , m_gpu_id(gpu_id)
    , m_pthread_name(fmt::format("gpu_thread_{}", gpu_id))
{
   solver_param.Clear();
   pthread_setname_np(pthread_self(), m_pthread_name.c_str());
}

Network::~Network() {
   // m_solver->net() = nullptr;
   spdlog::warn("thread {} ~Network() done", thread_name());
}
void Network::free_network(){
   // for (auto& blob : m_solver->net()->blobs() ) {
   //    free(blob.get());
   // }
   // spdlog::warn("m_solver->net() = nullptr");
   // m_solver->net() = nullptr;
   // spdlog::warn("m_solver->net() = nullptr done");
   // spdlog::warn("m_solver->Restore");
   // m_solver->Restore("./msi_iter_0.solverstate");
   // spdlog::warn("m_solver->Restore done");
   spdlog::warn("m_solver = nullptr");
   m_solver->net() = nullptr;
   m_solver = nullptr;
   spdlog::warn("m_solver= nullptr done");
}
// #define MEASURE_TIME

std::string thread_name() {
   char buff[100];
   pthread_getname_np(pthread_self(), buff, 100);
   return std::string(buff);
}

FullMove Network::do_move(const std::unique_ptr<IGame> &g, float prob) {
   caffe::Caffe::SetDevice(this->get_gpu_id());
   static constexpr auto output_neuron_count =  g_board_width * g_board_height * 4 * 10;
#ifdef MEASURE_TIME
   auto start_board_to_caffe_X = util::unix_time();
#endif
   g->board_to_caffe_X(m_neuron_input);
#ifdef MEASURE_TIME
   spdlog::debug("net: board_to_caffe_X lasted {}ms", util::unix_time() - start_board_to_caffe_X);
   auto start_copy_to_mutable_cpu_data = util::unix_time();
#endif
   std::copy(m_neuron_input.begin(), m_neuron_input.end(), m_solver->net()->blob_by_name("input_data")->mutable_cpu_data());
#ifdef MEASURE_TIME
   spdlog::debug("net: copy_to_mutable_cpu_data lasted {}ms", util::unix_time() - start_copy_to_mutable_cpu_data);
   auto start_copy_cpu_to_gpu = util::unix_time();
   m_solver->net()->blob_by_name("input_data")->gpu_data();
   spdlog::debug("net: copy_cpu_to_gpu lasted {}ms", util::unix_time() - start_copy_cpu_to_gpu);
   auto start_forward = util::unix_time();
#endif
   m_solver->net()->Forward();
#ifdef MEASURE_TIME
   spdlog::debug("net: forward lasted {}ms", util::unix_time() - start_forward);
   auto start_copy_gpu_to_cpu = util::unix_time();
   const float &mutable_cpu_data = *m_solver->net()->blob_by_name("output_probabs")->mutable_cpu_data();
#endif
   std::span<float> out_span(m_solver->net()->blob_by_name("output_probabs")->mutable_cpu_data(), output_neuron_count);
#ifdef MEASURE_TIME
   spdlog::debug("net: copy_gpu_to_cpu lasted {}ms", util::unix_time() - start_copy_gpu_to_cpu);
   auto start_decode_move = util::unix_time();
#endif
   auto move = decoder::decode_move(g, m_allowed_moves, out_span, prob);
#ifdef MEASURE_TIME
   spdlog::debug("net: decode_move lasted {}ms", util::unix_time() - start_decode_move);
#endif
   return move;
}

// void Network::train( const std::vector<training::OneGame> &data_set) {
//    caffe::Caffe::SetDevice(0);
//    // m_net_parameter.mutable_state()->set_phase(caffe::TRAIN);
//    // m_solver->net() = boost::shared_ptr<caffe::Net<float>>(new caffe::Net<float>(m_net_parameter));
//    spdlog::info("training network!");
//    for (const auto &game : data_set) {
//       for (const auto &move: game) {
         
//          std::copy(move.game_state.begin(), move.game_state.end(), m_solver->net()->blob_by_name("input_data")->mutable_cpu_data());
//          std::copy(move.children_visits.begin(), move.children_visits.end(), m_solver->net()->blob_by_name("label_probabs")->mutable_cpu_data());
//          *m_solver->net()->blob_by_name("label_value")->mutable_cpu_data() = move.reward;
         
//          // net_parameter.mutable_state()->set_phase(caffe::TRAIN);
//          // solver->net() = boost::shared_ptr<caffe::Net<float>>(new caffe::Net<float>(net_parameter));
         
//          // auto start_forward = util::unix_time();
//          // m_solver->net()->Forward();
//          // spdlog::debug("net: forward lasted {}ms", util::unix_time() - start_forward);
         
//          // auto start_backward = util::unix_time();
//          // m_solver->net()->Backward();
//          // spdlog::debug("net: backward lasted {}ms", util::unix_time() - start_backward);

//          auto start_step = util::unix_time();
//          for (int i = 50; i >= 0; --i) {
//             fmt::print("{} :\n", i);
//             fflush(stdout);
//             m_solver->Step(1);
//             fmt::print("loss={:9.2e}\n", *m_solver->history().at(m_solver->history().size()-1)->cpu_data());
//          }
//          spdlog::debug("net: 50 Steps lasted {}ms", util::unix_time() - start_step);
//       }
//    }
// }

static bool update_networks(uint64_t net_id) {
   try {
      auto net_it = g_networks.begin();
      std::advance(net_it, net_id);
      net_it->second->solver()->net().reset(new caffe::Net<float>("./proto/net_full_alphazero_40_res_blocks.prototxt", caffe::TEST));
      net_it->second->solver()->net()->CopyTrainedLayersFrom("./msi_iter_0.caffemodel");
   } catch (const std::exception& e) {
      spdlog::error("{}", e.what());
      return -1;
   }
   spdlog::info("network {} updated", net_id);
   return 0;
}

void Network::train( const std::array<training::OneGame*, g_batch_size> &data_set) {
   
   caffe::Caffe::set_mode(caffe::Caffe::GPU);
   caffe::Caffe::set_multiprocess(true);
   caffe::Caffe::set_solver_count(Eden_resources::get_gpus_count());
   caffe::Caffe::SetDevice(4);

   fmt::print("Training preparation\n");
   caffe::SolverParameter solver_param;
   caffe::ReadSolverParamsFromTextFileOrDie("./proto/solver_train.prototxt", &solver_param);
   fmt::print("model: {}\n", solver_param.net());
   caffe::SGDSolver<float>* solver = new caffe::SGDSolver<float>(solver_param);
   solver->net()->CopyTrainedLayersFrom("./msi_iter_0.caffemodel");
   
   // caffe::NetParameter net_parameter;
   // std::ifstream t("./proto/net_full_alphazero_40_res_blocks.prototxt");
   // std::string model((std::istreambuf_iterator<char>(t)),
   //                std::istreambuf_iterator<char>());
   // bool success = google::protobuf::TextFormat::ParseFromString(model, &net_parameter);
   // fmt::print("success={}\n", success);
   // if (success) {
   //    net_parameter.mutable_state()->set_phase(caffe::TRAIN);
   //    solver->net() = boost::shared_ptr<caffe::Net<float>>(new caffe::Net<float>(net_parameter));
   // }
   fmt::print("size={}\n", solver->net()->input_blobs().size() );
   for (int i = 0; i < solver->net()->input_blobs().size(); ++i)
      fmt::print("{}: size={}\n", i, solver->net()->input_blob_indices()[i] );
   
   caffe::Blob<float>* input_data_blob = solver->net()->blob_by_name("input_data").get();
   caffe::Blob<float>* label_value_blob = solver->net()->blob_by_name("label_value").get();
   caffe::Blob<float>* label_probabs_blob = solver->net()->blob_by_name("label_probabs").get();
   caffe::Blob<float>* loss_value_blob = solver->net()->blob_by_name("value_loss").get();
   caffe::Blob<float>* output_value_blob = solver->net()->blob_by_name("output_value").get();

   float* input_data_begin = input_data_blob->mutable_cpu_data();
   float* label_probabs_begin = label_probabs_blob->mutable_cpu_data();
   float* label_value_begin = label_value_blob->mutable_cpu_data();
   if (input_data_begin == nullptr || label_probabs_begin == nullptr || label_value_begin == nullptr) {
      fmt::print("empty mutable_cpu_data\n");
   }
   fmt::print("games_count={}\n", g_batch_size);
   fmt::print("moves_count={}\n", g_batch_size * data_set[0]->size());

   int i_debug = 0;
   for (const auto &game : data_set) {
      for (const auto &move : *game) {
         // memcpy(input_data_begin, move.game_state.data(), move.game_state.size() * sizeof(float));
         // memcpy(label_probabs_begin, move.children_visits.data(), move.children_visits.size() * sizeof(float));
         // memcpy(label_value_begin, &move.reward, 1 * sizeof(float));
         std::copy(move.game_state.begin(), move.game_state.end(), input_data_blob->mutable_cpu_data());
         std::copy(move.children_visits.begin(), move.children_visits.end(), label_probabs_blob->mutable_cpu_data());
         *label_value_blob->mutable_cpu_data() = move.reward;
         auto start_step = util::unix_time();
         // solver->Step(1);
         solver->net()->Forward();
         solver->net()->Backward();
         spdlog::error("net: loss={:9.5e}, value={:9.3e}, backward lasted {}ms", *loss_value_blob->cpu_data(), *output_value_blob->cpu_data(), util::unix_time() - start_step);
         if (++i_debug > 76) return;
      }
   }
   solver->Snapshot();
   std::vector<std::future<bool>> futures{g_networks.size()};
   for (uint64_t net_id = 0; net_id < g_networks.size(); ++net_id)
      futures[net_id] = std::async(std::launch::async, update_networks, net_id);
   for (uint64_t net_id = 0; net_id < g_networks.size(); ++net_id)
      if (futures[net_id].get()) spdlog::error("Network::train");
   free(solver);
   // spdlog::info("training network!");
   // // m_solver->net() = boost::shared_ptr<caffe::Net<float>>(new caffe::Net<float>(m_net_parameter));
   // fmt::print("input_blobs_size={}\n", m_solver->net()->input_blobs().size() );
   // for (int i = 0; i < m_solver->net()->input_blobs().size(); ++i)
   //    for (int j = 0; j < 4; ++j)
   //    fmt::print("{}: shape={}\n", i, m_solver->net()->input_blobs().at(i)->LegacyShape(j) );
   // for (const auto &game : data_set) {
   //    for (const auto &move: game) {
   //       auto start_copy = util::unix_time();
   //       std::copy(move.game_state.begin(), move.game_state.end(), m_solver->net()->blob_by_name("input_data")->mutable_cpu_data());
   //       std::copy(move.children_visits.begin(), move.children_visits.end(), m_solver->net()->blob_by_name("label_probabs")->mutable_cpu_data());
   //       *m_solver->net()->blob_by_name("label_value")->mutable_cpu_data() = move.reward;
   //       m_solver->net()->blob_by_name("input_data")->mutable_gpu_data();
   //       m_solver->net()->blob_by_name("label_probabs")->mutable_gpu_data();
   //       m_solver->net()->blob_by_name("label_value")->mutable_gpu_data();
   //       spdlog::debug("net: copy lasted {}ms", util::unix_time() - start_copy);
         
   //       // auto start_forward = util::unix_time();
   //       // if (nullptr == m_solver->net()) spdlog::error("nullptr == m_solver->net()");
   //       // m_solver->net()->Forward();
   //       // spdlog::debug("net: forward lasted {}ms", util::unix_time() - start_forward);
         
   //       // auto start_backward = util::unix_time();
   //       // m_solver->net()->Backward();
   //       // spdlog::debug("net: backward lasted {}ms", util::unix_time() - start_backward);

   //       auto start_step = util::unix_time();
   //       for (int i = 50; i >= 0; --i) {
   //          fmt::print("{} ", i);
   //          fflush(stdout);
   //          m_solver->Step(1);
   //          fmt::print("done: loss={:9.2e}\n", *m_solver->history().at(m_solver->history().size()-1)->cpu_data());
   //       }
   //       spdlog::debug("net: 50 Steps lasted {}ms", util::unix_time() - start_step);
   //    }
   // }
}

}// namespace carcassonne::rl