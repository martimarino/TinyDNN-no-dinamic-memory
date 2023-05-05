#include <iostream>
# include <chrono>

//#define CNN_USE_POSIT
#include "tiny_dnn/tiny_dnn.h"
using  ns = std::chrono::nanoseconds;
using  ms = std::chrono::milliseconds;
using get_time = std::chrono::steady_clock ;


int main(int argc, char **argv) {
  tiny_dnn::network<tiny_dnn::sequential> net;
  std::string model_path(argv[1]);
  std::string data_path(argv[2]);
  net.load(model_path); //load pre-trained model
  
  std::vector<tiny_dnn::label_t> vlab;
  std::vector<tiny_dnn::vec_t> vimg;
  std::vector<unsigned int> times;
  parse_binImagenet<32,32,3,uint8_t>(data_path,&vimg,&vlab,(tiny_dnn::float_t)-2,(tiny_dnn::float_t)2);
  int num = (argc < 4)? vimg.size():atoi(argv[3]);
  
  int successes = 0;
  auto start = get_time::now(); 
  double time_mean = 0;
  for(unsigned int i = 0; i < num; ++i) {
    auto inner_b = get_time::now();
    auto res = net.predict(vimg[i]);
    auto inner_e = get_time::now();
    std::vector<std::pair<double, unsigned int>> scores;
    for (int j = 0; j < 42; j++) scores.emplace_back(res[j], j);
    sort(scores.begin(), scores.end(), std::greater<std::pair<double, int>>());
    if(vlab[i] == scores[0].second) successes++;
    std::cout << "Inferencing " << i+1 << "/" << vimg.size() << "(Acc: " << (double)successes/double(i+1) << ")" << std::endl;
    std::clog << "\r" << i+1 << "/" << vimg.size();
    unsigned int tttt = std::chrono::duration_cast<ms>(inner_e - inner_b).count();
    time_mean+= double(tttt);
    times.push_back(tttt);
  }
  auto end = get_time::now();
  auto diff = end - start;
  make_test_report(vimg,successes,times,std::chrono::duration_cast<ms>(diff).count(),time_mean/double(num),"../../plot/test_perf_");
  std::cout << "Accuracy: "  << successes << "/" << vimg.size() << "\n";
  std::cout << "Time elapsed: " << std::chrono::duration_cast<ms>(diff).count() << "ms\n";
  std::cout << "Time mean: " <<  time_mean/double(num) << std::endl;
	return 0;
}