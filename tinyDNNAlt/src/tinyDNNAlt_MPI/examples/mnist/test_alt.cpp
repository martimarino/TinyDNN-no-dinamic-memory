#include <iostream>
# include <chrono>

//#define CNN_USE_POSIT
#include "tiny_dnn/tiny_dnn.h"
using  ns = std::chrono::nanoseconds;
using  ms = std::chrono::milliseconds;
using get_time = std::chrono::steady_clock ;




int main(int argc, char **argv) {
  std::cout << "PROGRAM STARTING!!!!!!!!!!!!!" << std::endl;
	tiny_dnn::network<tiny_dnn::sequential> net;
  if (argc < 3) {
    std::cout << "Usage example_mnist_train_alt path/to/network_model path/to/dataset" << std::endl;
    exit(-1);
  }
  std::string model_path(argv[1]);
  std::string data_path(argv[2]);
  float lowb = atof(argv[3]);
  float highb = atof(argv[4]);
  std::cout << "Loading model " << model_path << std::endl;
	net.load(model_path); //load pre-trained model
  std::cout << "Done..." << std::endl;
 	std::vector<tiny_dnn::label_t> test_labels;
	std::vector<tiny_dnn::vec_t> test_images;
  std::vector<unsigned int> times;
  tiny_dnn::parse_mnist_labels(data_path + "/t10k-labels.idx1-ubyte",
                               &test_labels);
  tiny_dnn::parse_mnist_images(data_path + "/t10k-images.idx3-ubyte",
                               &test_images, lowb, highb, 2, 2);
  int successes = 0;

  auto start = get_time::now(); 
  double stime = 0;
  int num = (argc > 5)? atoi(argv[5]):test_images.size();
  for(unsigned int i = 0; i < num; ++i) {
    auto inner_b = get_time::now();
    auto res = net.predict(test_images[i]);
    auto inner_e = get_time::now();
    std::vector<std::pair<double, unsigned int>> scores;
    for (int j = 0; j < 10; j++) scores.emplace_back(res[j], j);
    sort(scores.begin(), scores.end(), std::greater<std::pair<double, int>>());
    if(test_labels[i] == scores[0].second) successes++;
    std::cout << "Inferencing " << i+1 << "/" << test_images.size() << "(Acc: " << (double)successes/double(i+1) << ")" << std::endl;
    std::clog << "\r" << i+1 << "/" << test_images.size();
    unsigned int tttt = std::chrono::duration_cast<ms>(inner_e - inner_b).count();
    times.push_back(tttt);
    stime+=tttt;
  }
  std::cout << std::endl << stime/test_images.size() << std::endl;
  auto end = get_time::now();
  auto diff = end - start;
  make_test_report(test_images,successes,times,std::chrono::duration_cast<ms>(diff).count(),stime/double(test_images.size()),"../../plot/test_perf_");
  std::cout << "Accuracy: "  << successes << "/" << test_images.size() << "\n";
  std::cout << "Time elapsed: " << std::chrono::duration_cast<ms>(diff).count() << "ms\n";
	return 0;
}
