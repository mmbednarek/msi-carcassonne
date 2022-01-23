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

static constexpr const mb::u64 g_batch_size = 2048;

std::string thread_name();

class Network {
   std::unique_ptr<caffe::SGDSolver<float>> m_solver;
   std::vector<float> m_neuron_input;
   std::vector<bool> m_allowed_moves;
   int m_gpu_id;
   std::string m_pthread_name;

 public:
   Network(caffe::SolverParameter &&solver_param, int gpu_id);
   Network(Network &&) noexcept = delete;
   Network &operator=(Network &&) noexcept = delete;
   Network(const Network &other) = delete;
   Network &operator=(const Network &other) = delete;
   ~Network();

   FullMove do_move(const std::unique_ptr<IGame> &g, float prob);
   std::string get_thread_name() { return m_pthread_name; }
   std::unique_ptr<caffe::SGDSolver<float>>& solver() { return m_solver; }
   int get_gpu_id() { return m_gpu_id; }
   
   void train(const std::array<training::OneGame*, g_batch_size> &data_set);
   void free_network();

};

}

#endif//MSI_CARCASSONNE_EPISODE_H
