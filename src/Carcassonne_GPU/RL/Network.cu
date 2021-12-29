#ifdef __CUDACC__
#define DEVHOST __device__ __host__
#else
#define DEVHOST
#endif

#include <Carcassonne_GPU/Decoder/Decoder.h>
#include <Carcassonne_GPU/RL/Network.h>
#include <Util_GPU/Time.h>
#include <fmt/core.h>
#include <spdlog/spdlog.h>

namespace carcassonne::rl {

template<typename T, typename... ARGS>
DEVHOST constexpr boost::shared_ptr<T> make_shared(ARGS... args) {
   return boost::shared_ptr<T>(new T(args...));
}

DEVHOST Network::Network(const caffe::NetParameter &net_param, const caffe::SolverParameter &solver_param)
 : m_net(make_shared<caffe::Net<float>>(net_param))
 , m_solver(solver_param)
 , m_input(m_net->blob_by_name("input_data"))
 , m_output(m_net->blob_by_name("output_probas"))
 , m_allowed_moves(g_board_width * g_board_height * 4 * 14)
//  , m_label(m_net->blob_by_name("label_value"))
{
   m_solver.net() = m_net;
}

DEVHOST game::FullMove Network::do_move(game::Game &g, TileType tile, float prob)
{
   static constexpr auto output_neuron_count =  g_board_width * g_board_height * 4 * 10;
   g.board_to_caffe_X(m_neuron_input);
   std::copy(m_neuron_input.begin(), m_neuron_input.end(), m_input->mutable_cpu_data());

   spdlog::debug("forward start");
   m_net->Forward();
   spdlog::debug("forward stop");

   my_std::span<float> out_span(m_output->mutable_cpu_data(), output_neuron_count);
   auto move = decoder::decode_move(g, tile, m_allowed_moves, out_span, prob);
   return move;
}

}// namespace carcassonne::rl