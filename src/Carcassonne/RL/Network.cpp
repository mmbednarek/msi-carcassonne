#include <Carcassonne/Decoder/Decoder.h>
#include <Carcassonne/RL/Network.h>
#include <Eden_resources/Ngpus_Ncpus.h>
#include <Util/Time.h>
#include <fmt/core.h>
#include <span>
#include <google/protobuf/text_format.h>
// #define SPDLOG_FMT_EXTERNAL
#include <spdlog/spdlog.h>
#include <pthread.h>

namespace carcassonne::ai::rl {

template<typename T, typename... ARGS>
constexpr boost::shared_ptr<T> make_shared(ARGS... args) {
   return boost::shared_ptr<T>(new T(args...));
}

Network::Network(
   const caffe::NetParameter &net_parameter,
   const caffe::SolverParameter &solver_param,
   int gpu_id )
    : m_solver(solver_param)
    , m_net_parameter(net_parameter)
    , m_input_data(m_solver.net()->blob_by_name("input_data"))
    , m_output_probabs(m_solver.net()->blob_by_name("output_probabs"))
    , m_output_value(m_solver.net()->blob_by_name("output_value"))
    , m_label_probabs(m_solver.net()->blob_by_name("label_probabs"))
    , m_label_value(m_solver.net()->blob_by_name("label_value"))
    , m_allowed_moves(g_board_width * g_board_height * 4 * 14, false)
    , m_gpu_id(gpu_id)
    , m_pthread_name(fmt::format("gpu_thread_{}", gpu_id))
{
   pthread_setname_np(pthread_self(), m_pthread_name.c_str());
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
   std::copy(m_neuron_input.begin(), m_neuron_input.end(), m_input_data->mutable_cpu_data());
#ifdef MEASURE_TIME
   spdlog::debug("net: copy_to_mutable_cpu_data lasted {}ms", util::unix_time() - start_copy_to_mutable_cpu_data);
   auto start_copy_cpu_to_gpu = util::unix_time();
   m_input_data->gpu_data();
   spdlog::debug("net: copy_cpu_to_gpu lasted {}ms", util::unix_time() - start_copy_cpu_to_gpu);
   auto start_forward = util::unix_time();
#endif
   m_solver.net()->Forward();
#ifdef MEASURE_TIME
   spdlog::debug("net: forward lasted {}ms", util::unix_time() - start_forward);
   auto start_copy_gpu_to_cpu = util::unix_time();
   const float &mutable_cpu_data = *m_output_probabs->mutable_cpu_data();
#endif
   std::span<float> out_span(m_output_probabs->mutable_cpu_data(), output_neuron_count);
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
//    // m_solver.net() = boost::shared_ptr<caffe::Net<float>>(new caffe::Net<float>(m_net_parameter));
//    spdlog::info("training network!");
//    for (const auto &game : data_set) {
//       for (const auto &move: game) {
         
//          std::copy(move.game_state.begin(), move.game_state.end(), m_input_data->mutable_cpu_data());
//          std::copy(move.children_visits.begin(), move.children_visits.end(), m_label_probabs->mutable_cpu_data());
//          *m_label_value->mutable_cpu_data() = move.reward;
         
//          // net_parameter.mutable_state()->set_phase(caffe::TRAIN);
//          // solver->net() = boost::shared_ptr<caffe::Net<float>>(new caffe::Net<float>(net_parameter));
         
//          // auto start_forward = util::unix_time();
//          // m_solver.net()->Forward();
//          // spdlog::debug("net: forward lasted {}ms", util::unix_time() - start_forward);
         
//          // auto start_backward = util::unix_time();
//          // m_solver.net()->Backward();
//          // spdlog::debug("net: backward lasted {}ms", util::unix_time() - start_backward);

//          auto start_step = util::unix_time();
//          for (int i = 50; i >= 0; --i) {
//             fmt::print("{} :\n", i);
//             fflush(stdout);
//             m_solver.Step(1);
//             fmt::print("loss={:9.2e}\n", *m_solver.history().at(m_solver.history().size()-1)->cpu_data());
//          }
//          spdlog::debug("net: 50 Steps lasted {}ms", util::unix_time() - start_step);
//       }
//    }
// }



void Network::train( const std::vector<training::OneGame> &data_set) {
   
   
   caffe::Caffe::set_mode(caffe::Caffe::GPU);
   caffe::Caffe::set_multiprocess(true);
   caffe::Caffe::set_solver_count(Eden_resources::get_gpus_count());

   fmt::print("Training preparation\n");
   caffe::SolverParameter solver_param;
   caffe::ReadSolverParamsFromTextFileOrDie("./proto/solver_train.prototxt", &solver_param);
   fmt::print("model: {}\n", solver_param.net());
   caffe::SGDSolver<float>* solver = new caffe::SGDSolver<float>(solver_param);

   caffe::NetParameter net_parameter;

   std::ifstream t("./proto/net_full_alphazero_40_res_blocks.prototxt");
   std::string model((std::istreambuf_iterator<char>(t)),
                  std::istreambuf_iterator<char>());
   bool success = google::protobuf::TextFormat::ParseFromString(model, &net_parameter);
   fmt::print("success={}\n", success);
   if (success) {
      net_parameter.mutable_state()->set_phase(caffe::TRAIN);
      solver->net() = boost::shared_ptr<caffe::Net<float>>(new caffe::Net<float>(net_parameter));
   }
   fmt::print("size={}\n", solver->net()->input_blobs().size() );
   for (int i = 0; i < solver->net()->input_blobs().size(); ++i)
      fmt::print("{}: size={}\n", i, solver->net()->input_blob_indices()[i] );
   
   boost::shared_ptr<caffe::Blob<float>> input_data_blob = solver->net()->blob_by_name("input_data");
   boost::shared_ptr<caffe::Blob<float>> label_value_blob = solver->net()->blob_by_name("label_value");
   boost::shared_ptr<caffe::Blob<float>> label_probabs_blob = solver->net()->blob_by_name("label_probabs");
   boost::shared_ptr<caffe::Blob<float>> loss_value_blob = solver->net()->blob_by_name("value_loss");
   boost::shared_ptr<caffe::Blob<float>> output_value_blob = solver->net()->blob_by_name("output_value");

   float* input_data_begin = input_data_blob->mutable_cpu_data();
   float* label_probabs_begin = label_probabs_blob->mutable_cpu_data();
   float* label_value_begin = label_value_blob->mutable_cpu_data();
   if (input_data_begin == nullptr || label_probabs_begin == nullptr || label_value_begin == nullptr) {
      fmt::print("empty mutable_cpu_data\n");
   }
   for (const auto &game : data_set) {
      for (const auto &move : game) {
         // memcpy(input_data_begin, move.game_state.data(), move.game_state.size() * sizeof(float));
         // memcpy(label_probabs_begin, move.children_visits.data(), move.children_visits.size() * sizeof(float));
         // memcpy(label_value_begin, &move.reward, 1 * sizeof(float));
         std::copy(move.game_state.begin(), move.game_state.end(), input_data_blob->mutable_cpu_data());
         std::copy(move.children_visits.begin(), move.children_visits.end(), label_probabs_blob->mutable_cpu_data());
         *label_value_blob->mutable_cpu_data() = move.reward;
         auto start_step = util::unix_time();
         solver->Step(100);
         fmt::print("done: loss={:9.2e}, value={:9.2e}\n", *loss_value_blob->cpu_data(), *output_value_blob->cpu_data());
         spdlog::debug("net: 100 Steps lasted {}ms", util::unix_time() - start_step);
         // }
      }
   }


   // spdlog::info("training network!");
   // // m_solver.net() = boost::shared_ptr<caffe::Net<float>>(new caffe::Net<float>(m_net_parameter));
   // fmt::print("input_blobs_size={}\n", m_solver.net()->input_blobs().size() );
   // for (int i = 0; i < m_solver.net()->input_blobs().size(); ++i)
   //    for (int j = 0; j < 4; ++j)
   //    fmt::print("{}: shape={}\n", i, m_solver.net()->input_blobs().at(i)->LegacyShape(j) );
   // for (const auto &game : data_set) {
   //    for (const auto &move: game) {
   //       auto start_copy = util::unix_time();
   //       std::copy(move.game_state.begin(), move.game_state.end(), m_input_data->mutable_cpu_data());
   //       std::copy(move.children_visits.begin(), move.children_visits.end(), m_label_probabs->mutable_cpu_data());
   //       *m_label_value->mutable_cpu_data() = move.reward;
   //       m_input_data->mutable_gpu_data();
   //       m_label_probabs->mutable_gpu_data();
   //       m_label_value->mutable_gpu_data();
   //       spdlog::debug("net: copy lasted {}ms", util::unix_time() - start_copy);
         
   //       // auto start_forward = util::unix_time();
   //       // if (nullptr == m_solver.net()) spdlog::error("nullptr == m_solver.net()");
   //       // m_solver.net()->Forward();
   //       // spdlog::debug("net: forward lasted {}ms", util::unix_time() - start_forward);
         
   //       // auto start_backward = util::unix_time();
   //       // m_solver.net()->Backward();
   //       // spdlog::debug("net: backward lasted {}ms", util::unix_time() - start_backward);

   //       auto start_step = util::unix_time();
   //       for (int i = 50; i >= 0; --i) {
   //          fmt::print("{} ", i);
   //          fflush(stdout);
   //          m_solver.Step(1);
   //          fmt::print("done: loss={:9.2e}\n", *m_solver.history().at(m_solver.history().size()-1)->cpu_data());
   //       }
   //       spdlog::debug("net: 50 Steps lasted {}ms", util::unix_time() - start_step);
   //    }
   // }
}

}// namespace carcassonne::rl