#ifndef MSI_CARCASSONNE_EPISODE_H
#define MSI_CARCASSONNE_EPISODE_H
#define DISABLE_HIP_PROFILE
#include <caffe/caffe.hpp>
#include <caffe/filler.hpp>
#include <caffe/net.hpp>
#include <caffe/util/math_functions.hpp>
#include <caffe/sgd_solvers.hpp>
#include <Carcassonne/IGame.h>
#include <Carcassonne/Training/TrainingSet.h>

namespace carcassonne::ai::rl {

std::string thread_name();

class Network {
   caffe::SGDSolver<float> m_solver;
   boost::shared_ptr<caffe::Blob<float>> m_input_data;
   boost::shared_ptr<caffe::Blob<float>> m_output_probas;
   boost::shared_ptr<caffe::Blob<float>> m_output_value;
   boost::shared_ptr<caffe::Blob<float>> m_label_probas;
   boost::shared_ptr<caffe::Blob<float>> m_label_value;
   std::vector<float> m_neuron_input;
   std::vector<bool> m_allowed_moves;
   int m_gpu_id;
   std::string m_pthread_name;

 public:
   Network(const caffe::NetParameter &net_parameter, const caffe::SolverParameter &solver_param, int gpu_id);
   Network(Network &&) noexcept = delete;
   Network &operator=(Network &&) noexcept = delete;
   Network(const Network &other) = delete;
   Network &operator=(const Network &other) = delete;

   FullMove do_move(const std::unique_ptr<IGame> &g, float prob);
   std::string get_thread_name() { return m_pthread_name; }
   caffe::SGDSolver<float>& solver() { return m_solver; }
   int get_gpu_id() { return m_gpu_id; }
   void train(const std::vector<training::OneGame> &data_set);

};

}

#endif//MSI_CARCASSONNE_EPISODE_H
