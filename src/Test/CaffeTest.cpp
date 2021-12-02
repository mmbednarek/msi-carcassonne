#include <fmt/core.h>
#include <caffe/caffe.hpp>
#include <caffe/filler.hpp>
#include <caffe/layers/memory_data_layer.hpp>
#include <caffe/net.hpp>
#include <caffe/util/math_functions.hpp>
#include <caffe/sgd_solvers.hpp>
#include <google/protobuf/text_format.h>

namespace caffe {

void test_caffe1() {
  SyncedMemory data_a(10 * sizeof(unsigned int));
  Caffe::set_random_seed(1701);
  caffe_rng_bernoulli(10, 0.5, static_cast<int *>(data_a.mutable_cpu_data()));
  for (int i = 0; i < 10; ++i) {
    printf("%d ", ((const unsigned int *)(data_a.cpu_data()))[i]);
  }
  printf("\n");
}

}  // namespace caffe

void test_net() {
  fmt::print("Passing the input to it for training\n");
  float *data = new float[64 * 1 * 1 * 3 * 400];
  float *label = new float[64 * 1 * 1 * 1 * 400];
  for (int i = 0; i < 64 * 1 * 1 * 400; ++i) {
    int a = rand() % 2;
    int b = rand() % 2;
    int c = a ^ b;
    data[i * 2 + 0] = a;
    data[i * 2 + 1] = b;
    label[i] = c;
  }

  if (caffe::Caffe::mode() == caffe::Caffe::CPU) printf("mode=CPU\n");
  if (caffe::Caffe::mode() == caffe::Caffe::GPU) printf("mode=GPU\n");
  caffe::Caffe::set_mode(caffe::Caffe::GPU);
  if (caffe::Caffe::mode() == caffe::Caffe::CPU) printf("mode=CPU\n");
  if (caffe::Caffe::mode() == caffe::Caffe::GPU) printf("mode=GPU\n");
  caffe::Caffe::SetDevice(0);

  fmt::print("Training preparation\n");
  caffe::SolverParameter solver_param;
  caffe::ReadSolverParamsFromTextFileOrDie("./solver.prototxt", &solver_param);
  fmt::print("model: {}\n", solver_param.net());
//   std::shared_ptr<caffe::Solver<float>> solver(
//       caffe::SolverRegistry<float>::CreateSolver(solver_param));
  caffe::SGDSolver<float>* solver = new caffe::SGDSolver<float>(solver_param);
  caffe::MemoryDataLayer<float> *dataLayer_trainnet =
      (caffe::MemoryDataLayer<float>
           *)(solver->net()->layer_by_name("inputdata").get());

  dataLayer_trainnet->Reset(data, label, 25600);

  fmt::print("Training:\n");
  for (int i = 49; i >= 0; --i) {
    fmt::print("{} :", i);
    fflush(stdout);
    solver->Step(1);
    for (int layer_id = 0; layer_id < solver->history().size(); ++layer_id) 
      fmt::print("loss{}={:9.2e} ", layer_id, *solver->history().at(layer_id)->cpu_data());
    fmt::print("\n");
  }

  fmt::print("Testing preparation\n");
  std::shared_ptr<caffe::Net<float>> testnet;
  testnet.reset(new caffe::Net<float>("./model.prototxt", caffe::TEST));
  testnet->CopyTrainedLayersFrom("XOR_iter_50.caffemodel");

  fmt::print("Passing the input to it for testing\n");
  float testab[] = {0, 0, 0, 1, 1, 0, 1, 1};
  float testc[] = {0, 1, 1, 0};
  caffe::MemoryDataLayer<float> *dataLayer_testnet =
      (caffe::MemoryDataLayer<float> *)(testnet->layer_by_name("test_inputdata")
                                            .get());
  dataLayer_testnet->Reset(testab, testc, 4);

  fmt::print("Testing\n");
  testnet->Forward();

  boost::shared_ptr<caffe::Blob<float>> output_layer =
      testnet->blob_by_name("output");
  const float *begin = output_layer->cpu_data();
  const float *end = begin + 4;

  fmt::print("Move data to a vector\n");
  std::vector<float> result(begin, end);
  for (int i = 0; i < result.size(); ++i) {
    printf("input: %d xor %d,  truth: %f result by nn: %f\n",
           (int)testab[i * 2 + 0], (int)testab[i * 2 + 1], testc[i], result[i]);
  }
  free(data);
  free(label);
}

#include <iostream>
#include <string>
#include <filesystem>
#include <unistd.h>

namespace fs = std::filesystem;
using std::cout; using std::cin;
using std::endl; using std::string;
using fs::current_path;

int main() {
    char tmp[256];
    getcwd(tmp, 256);
    cout << "Current working directory: " << tmp << endl;
    std::string path = "./";
    for (const auto & entry : fs::directory_iterator(path))
        std::cout << entry.path() << std::endl;

  google::InitGoogleLogging("./out/log.txt");
  caffe::test_caffe1();
  test_net();

  return 0;
}
