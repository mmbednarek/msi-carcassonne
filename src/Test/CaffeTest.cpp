#include <fmt/core.h>
#include <caffe/caffe.hpp>
#include <caffe/filler.hpp>
#include <caffe/layers/input_layer.hpp>
#include <caffe/net.hpp>
#include <caffe/util/math_functions.hpp>
#include <caffe/sgd_solvers.hpp>
#include <google/protobuf/text_format.h>
#include <string>
#include <fstream>
#include <streambuf>

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
  // float *data = new float[64 * 1 * 1 * 3 * 400];
  // float *label = new float[64 * 1 * 1 * 1 * 400];
  // for (int i = 0; i < 64 * 1 * 1 * 400; ++i) {
  //   int a = rand() % 2;
  //   int b = rand() % 2;
  //   int c = a ^ b;
  //   data[i * 2 + 0] = a;
  //   data[i * 2 + 1] = b;
  //   label[i] = c;
  // }
  const size_t batch_size = 7;
  const size_t samples_count = 180;
  const size_t size_S = batch_size * 41 * 41 * 86 * samples_count;
  const size_t size_P = batch_size * 41 * 41 * 40 * samples_count;
  const size_t size_z = batch_size * 1 * 1 * 1 * samples_count;
  // std::array<float, size_S> data;
  // std::array<float, size_P> probabilities; // labels
  // std::array<float, size_z> values;
  float *data = new float[size_S];
  float *test_data = new float[size_S];
  float *probabilities = new float[size_P];
  float *values = new float[size_z];
  memset(data, 0, size_S * sizeof(float));
  memset(probabilities, 0, size_P * sizeof(float));
  memset(values, 0, size_z * sizeof(float));
  for (int i = 0; i < size_z; ++i) {
    for (int j = 0; j < size_S/size_z; ++j) {
      data[j + i * size_z] = j % 2;
      test_data[j + i * size_z] = (j+1) % 2;
    }
    for (int j = 0; j < size_P/size_z; ++j)
      probabilities[j + i * size_z] = .5;
    values[i] = (i % 3) + 1;
  }
  fmt::print("generating done\n");
  
  if (caffe::Caffe::mode() == caffe::Caffe::CPU) printf("mode=CPU\n");
  if (caffe::Caffe::mode() == caffe::Caffe::GPU) printf("mode=GPU\n");
  caffe::Caffe::set_mode(caffe::Caffe::CPU);
  if (caffe::Caffe::mode() == caffe::Caffe::CPU) printf("mode=CPU\n");
  if (caffe::Caffe::mode() == caffe::Caffe::GPU) printf("mode=GPU\n");
  caffe::Caffe::SetDevice(0);

  fmt::print("Training preparation\n");
  caffe::SolverParameter solver_param;
  caffe::ReadSolverParamsFromTextFileOrDie("./solver.prototxt", &solver_param);
  fmt::print("model: {}\n", solver_param.net());
  caffe::SGDSolver<float>* solver = new caffe::SGDSolver<float>(solver_param);

  caffe::NetParameter net_parameter;

  std::ifstream t("./net_tic_tac_6_4_2_res_block.prototxt");
  std::string model((std::istreambuf_iterator<char>(t)),
                 std::istreambuf_iterator<char>());
  bool success = google::protobuf::TextFormat::ParseFromString(model, &net_parameter);
  fmt::print("success={}\n", success);
  if (success){
    net_parameter.mutable_state()->set_phase(caffe::TRAIN);
    solver->net() = boost::shared_ptr<caffe::Net<float>>(new caffe::Net<float>(net_parameter));
  }
  fmt::print("size={}\n", solver->net()->input_blobs().size() );
  for (int i = 0; i < solver->net()->input_blobs().size(); ++i)
    fmt::print("{}: size={}\n", i, solver->net()->input_blob_indices()[i] );
  
  boost::shared_ptr<caffe::Blob<float>> input_data_blob = solver->net()->blob_by_name("input_data");
  boost::shared_ptr<caffe::Blob<float>> output_data_blob = solver->net()->blob_by_name("output_probas");
  boost::shared_ptr<caffe::Blob<float>> label_value_blob = solver->net()->blob_by_name("label_value");

  float* input_data_begin = input_data_blob->mutable_cpu_data();
  float* output_probas_begin = input_data_blob->mutable_cpu_data();
  float* label_value_begin = label_value_blob->mutable_cpu_data();
  if (input_data_begin == nullptr || output_probas_begin == nullptr || label_value_begin == nullptr) {
    fmt::print("empty mutable_cpu_data\n");
  }
  memcpy(input_data_begin, data, size_S / samples_count * sizeof(float));
  memcpy(output_probas_begin, probabilities, size_P / samples_count * sizeof(float));
  memcpy(label_value_begin, values, size_z / samples_count * sizeof(float));

  fmt::print("Training:\n");
  for (int i = 50; i >= 0; --i) {
    fmt::print("{} :", i);
    fflush(stdout);
    solver->Step(1);
    for (int layer_id = 0; layer_id < solver->history().size(); ++layer_id) 
      fmt::print("loss{}={:9.2e} ", layer_id, *solver->history().at(layer_id)->cpu_data());
    fmt::print("\n");
  }

  fmt::print("Testing preparation\n");
  std::shared_ptr<caffe::Net<float>> testnet;
  testnet.reset(new caffe::Net<float>("./net_tic_tac_6_4_2_res_block.prototxt", caffe::TEST));
  testnet->CopyTrainedLayersFrom("XOR_iter_50.caffemodel");

  fmt::print("Passing the input to it for testing\n");
  memcpy(input_data_begin, test_data, size_S / samples_count * sizeof(float));

  fmt::print("Testing\n");
  testnet->Forward();

  boost::shared_ptr<caffe::Blob<float>> output_probabs_layer =
      testnet->blob_by_name("output_probas");
  const float *begin_probabs = output_probabs_layer->cpu_data();
  const float *end_probabs = begin_probabs + size_S / samples_count / batch_size;

  fmt::print("Result probabilities\n");
  std::vector<float> result_probabs(begin_probabs, end_probabs);
  for (int i = 0; i < result_probabs.size(); ++i) {
    fmt::print("{:7.2e} ", result_probabs[i]);
  }
  fmt::print("\n");
  boost::shared_ptr<caffe::Blob<float>> output_value_layer =
      testnet->blob_by_name("output_value");
  const float *begin_value = output_probabs_layer->cpu_data();
  const float *end_value = begin_value + size_z / samples_count / batch_size;

  fmt::print("Result value\n");
  std::vector<float> result_value(begin_value, end_value);
  for (int i = 0; i < result_value.size(); ++i) {
    fmt::print("{:7.2e} ", result_value[i]);
  }
  fmt::print("\n");


  // free(data);
  // free(label);
}

#include <iostream>
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
