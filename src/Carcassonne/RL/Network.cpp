#include <Carcassonne/Decoder/Decoder.h>
#include <Carcassonne/RL/Network.h>
#include <Util/Time.h>
#include <fmt/core.h>
#include <span>
// #define SPDLOG_FMT_EXTERNAL
#include <spdlog/spdlog.h>
#include <pthread.h>

namespace carcassonne::ai::rl {

template<typename T, typename... ARGS>
constexpr boost::shared_ptr<T> make_shared(ARGS... args) {
   return boost::shared_ptr<T>(new T(args...));
}

std::map<std::thread::id, std::unique_ptr<Network>> g_networks;

Network::Network(
   const caffe::NetParameter &net_parameter,
   const caffe::SolverParameter &solver_param,
   int gpu_id )
    : m_solver(solver_param)
    , m_input(m_solver.net()->blob_by_name("input_data"))
    , m_output(m_solver.net()->blob_by_name("output_probas"))
    , m_label(m_solver.net()->blob_by_name("output_value"))
    , m_allowed_moves(g_board_width * g_board_height * 4 * 14, false)
    , m_gpu_id(gpu_id)
    , m_pthread_name(fmt::format("gpu_thread_{}", gpu_id))
{
   pthread_setname_np(pthread_self(), m_pthread_name.c_str());
   // caffe::Caffe::SetDevice(gpu_id);
}

// #define MEASURE_TIME

std::string thread_name() {
   char buff[100];
   pthread_getname_np(pthread_self(), buff, 100);
   return std::string(buff);
}

FullMove Network::do_move(const std::unique_ptr<IGame> &g, float prob, std::string tid_name) {
   spdlog::debug("thread {}: Network({})::do_move, Network.gpu_id={}, parent_caller_thread_id={}", thread_name(), this->get_thread_name(), this->get_gpu_id(), tid_name);
   caffe::Caffe::SetDevice(this->get_gpu_id());
   spdlog::debug("ok3.1");
   static constexpr auto output_neuron_count =  g_board_width * g_board_height * 4 * 10;
#ifdef MEASURE_TIME
   auto start_board_to_caffe_X = util::unix_time();
#endif
   if (nullptr == g) {
      spdlog::debug("thread {}: Network::do_move: nullptr == g", thread_name());
      return FullMove{};
   }
   spdlog::debug("thread {}: Network::do_move: nullptr != g", thread_name());
   g->board_to_caffe_X(m_neuron_input);
   spdlog::debug("ok3.2");
#ifdef MEASURE_TIME
   spdlog::debug("net: board_to_caffe_X lasted {}ms", util::unix_time() - start_board_to_caffe_X);
   auto start_copy_to_mutable_cpu_data = util::unix_time();
#endif
   if (nullptr == m_input) {
      spdlog::debug("thread {}: Network::do_move: nullptr == m_input", thread_name());
      return FullMove{};
   }
   if (nullptr ==  m_input->mutable_cpu_data()) {
      spdlog::debug("thread {}: Network::do_move: nullptr == m_input", thread_name());
      return FullMove{};
   }
   std::copy(m_neuron_input.begin(), m_neuron_input.end(), m_input->mutable_cpu_data());
   spdlog::debug("ok44");
   // std::copy(m_neuron_input.begin(), m_neuron_input.begin()+85'731, m_input->mutable_cpu_data());
#ifdef MEASURE_TIME
   spdlog::debug("net: copy_to_mutable_cpu_data lasted {}ms", util::unix_time() - start_copy_to_mutable_cpu_data);
   auto start_copy_cpu_to_gpu = util::unix_time();
#endif
   // m_input->gpu_data();
   spdlog::debug("ok55");
#ifdef MEASURE_TIME
   spdlog::debug("net: copy_cpu_to_gpu lasted {}ms", util::unix_time() - start_copy_cpu_to_gpu);
   auto start_forward = util::unix_time();
#endif
   // caffe::Caffe::SetDevice(gpu_id);
   m_solver.net()->Forward();
   spdlog::debug("ok66");
#ifdef MEASURE_TIME
   spdlog::debug("net: forward lasted {}ms", util::unix_time() - start_forward);

   auto start_copy_gpu_to_cpu = util::unix_time();
#endif
   const float &mutable_cpu_data = *m_output->mutable_cpu_data();
   spdlog::debug("ok77");
   // thrust::host_vector<float> probabs(m_output->mutable_cpu_data(), m_output->mutable_cpu_data() + output_neuron_count);
   std::span<float> out_span(m_output->mutable_cpu_data(), output_neuron_count);
   spdlog::debug("ok8");
#ifdef MEASURE_TIME
   spdlog::debug("net: copy_gpu_to_cpu lasted {}ms", util::unix_time() - start_copy_gpu_to_cpu);
   auto start_decode_move = util::unix_time();
#endif

   auto move = decoder::decode_move(g, m_allowed_moves, out_span, prob);
   spdlog::debug("ok9");
#ifdef MEASURE_TIME
   spdlog::debug("net: decode_move lasted {}ms", util::unix_time() - start_decode_move);
#endif
   return move;
}

}// namespace carcassonne::rl