#include <Carcassonne/Decoder/Decoder.h>
#include <Carcassonne/RL/Network.h>

namespace carcassonne::rl {

template<typename T, typename... ARGS>
constexpr boost::shared_ptr<T> make_shared(ARGS... args) {
   return boost::shared_ptr<T>(new T(args...));
}

Network::Network(const caffe::NetParameter &net_param, const caffe::SolverParameter &solver_param) : m_net(make_shared<caffe::Net<float>>(net_param)),
                                                                                                     m_solver(solver_param),
                                                                                                     m_input(m_net->blob_by_name("input_data")),
                                                                                                     m_output(m_net->blob_by_name("output_probas")),
//                                                                                                     m_label(m_net->blob_by_name("label_value")),
                                                                                                     m_allowed_moves(g_board_width * g_board_height * 4 * 14) {
   m_solver.net() = m_net;
}

FullMove Network::do_move(IGame &g, TileType tile, float prob) {
   static constexpr auto output_neuron_count =  g_board_width * g_board_height * 4 * 10;
   g.board_to_caffe_X(m_neuron_output);
   std::copy(m_neuron_output.begin(), m_neuron_output.end(), m_input->mutable_cpu_data());

   m_net->Forward();
   std::span<float> out_span(m_output->mutable_cpu_data(), output_neuron_count);
   return decoder::decode_move(g, tile, m_allowed_moves, out_span, prob);
}

}// namespace carcassonne::rl