#include <Carcassonne/Decoder/Decoder.h>
#include <Carcassonne/RL/Network.h>
#include <Util/Time.h>
#include <fmt/core.h>
#include <thrust/host_vector.h>
#include <spdlog/spdlog.h>

namespace carcassonne::rl {

template<typename T, typename... ARGS>
constexpr boost::shared_ptr<T> make_shared(ARGS... args) {
   return boost::shared_ptr<T>(new T(args...));
}

Network::Network(const caffe::NetParameter &net_param, const caffe::SolverParameter &solver_param) : m_net(make_shared<caffe::Net<float>>(net_param)),
                                                                                                     m_solver(solver_param),
                                                                                                     m_input(m_net->blob_by_name("input_data")),
                                                                                                     m_output(m_net->blob_by_name("output_probas")),
                                                                                                     m_label(m_net->blob_by_name("output_value")),
                                                                                                     m_allowed_moves(g_board_width * g_board_height * 4 * 14, false)
{
   m_solver.net() = m_net;
}

FullMove Network::do_move(const std::unique_ptr<IGame> &g, float prob) {
   static constexpr auto output_neuron_count =  g_board_width * g_board_height * 4 * 10;

   auto start_board_to_caffe_X = util::unix_time();
   g->board_to_caffe_X(m_neuron_input);
   spdlog::debug("net: board_to_caffe_X lasted {}ms", util::unix_time() - start_board_to_caffe_X);

   auto start_copy_to_mutable_cpu_data = util::unix_time();
   // std::copy(m_neuron_input.begin(), m_neuron_input.end(), m_input->mutable_cpu_data());
   std::copy(m_neuron_input.begin(), m_neuron_input.begin()+85'731, m_input->mutable_cpu_data());
   spdlog::debug("net: copy_to_mutable_cpu_data lasted {}ms", util::unix_time() - start_copy_to_mutable_cpu_data);

   auto start_copy_cpu_to_gpu = util::unix_time();
   m_input->gpu_data();
   spdlog::debug("net: copy_cpu_to_gpu lasted {}ms", util::unix_time() - start_copy_cpu_to_gpu);

   auto start_forward = util::unix_time();
   m_net->Forward();
   spdlog::debug("net: forward lasted {}ms", util::unix_time() - start_forward);

   // std::span<float> out_span(m_output->mutable_cpu_data(), output_neuron_count);
   // thrust::host_vector<float> probabs(m_output->mutable_cpu_data(), m_output->mutable_cpu_data() + output_neuron_count);
   auto start_copy_gpu_to_cpu = util::unix_time();
   const float &mutable_cpu_data = *m_output->mutable_cpu_data();
   spdlog::debug("net: copy_gpu_to_cpu lasted {}ms", util::unix_time() - start_copy_gpu_to_cpu);
   
   thrust::host_vector<float> probabs( output_neuron_count, mutable_cpu_data );
   // fmt::print("\n$$$$$$$$$$$ probabs:\n");
   // for (int i = probabs.size(); i > probabs.size() - 100; --i)
   //    fmt::print("{}:{}\t", i, probabs[i]);
   // fmt::print("\n########### blob:\n");
   // for (int i = probabs.size(); i > probabs.size() - 100; --i)
   //    fmt::print("{}:{}\t", i, *(m_output->mutable_cpu_data() + i));
   // fmt::print("\n");
   auto start_decode_move = util::unix_time();
   auto move = decoder::decode_move(g, m_allowed_moves, probabs, prob);
   spdlog::debug("net: decode_move lasted {}ms", util::unix_time() - start_decode_move);
   return move;
}

}// namespace carcassonne::rl