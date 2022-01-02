#ifndef MSI_CARCASSONNE_EPISODE_H
#define MSI_CARCASSONNE_EPISODE_H
#define DISABLE_HIP_PROFILE
#include <caffe/caffe.hpp>
#include <caffe/filler.hpp>
#include <caffe/net.hpp>
#include <caffe/util/math_functions.hpp>
#include <caffe/sgd_solvers.hpp>
#include <Carcassonne/IGame.h>

namespace carcassonne::rl {

class Network {
   boost::shared_ptr<caffe::Net<float>> m_net;
   caffe::SGDSolver<float> m_solver;
   boost::shared_ptr<caffe::Blob<float>> m_input;
   boost::shared_ptr<caffe::Blob<float>> m_output;
   boost::shared_ptr<caffe::Blob<float>> m_label;
   std::vector<float> m_neuron_input;
   std::vector<bool> m_allowed_moves;

 public:
   Network(const caffe::NetParameter &net_parameter, const caffe::SolverParameter &solver_param);
   Network(Network &&) noexcept = default;
   Network &operator=(Network &&) noexcept = default;
   Network(const Network &other) = delete;
   Network &operator=(const Network &other) = delete;

   FullMove do_move(const std::unique_ptr<IGame> &g, float prob);

};

}

#endif//MSI_CARCASSONNE_EPISODE_H
