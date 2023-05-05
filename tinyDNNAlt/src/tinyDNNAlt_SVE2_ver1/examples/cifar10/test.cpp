/*
    Copyright (c) 2013, Taiga Nomi and the respective contributors
    All rights reserved.

    Use of this source code is governed by a BSD-style license that can be found
    in the LICENSE file.
*/
#include <iostream>

#include "tiny_dnn/tiny_dnn.h"
using  ns = std::chrono::nanoseconds;
using  ms = std::chrono::milliseconds;
using get_time = std::chrono::steady_clock ;



void recognize(const std::string &net, const std::string &src_filename,int numtest) {
  startLog(false);
  tiny_dnn::network<tiny_dnn::sequential> nn;
  nn.load(net);
  endLog("cifar_network_weights.txt");
  /*====================================================*/

  std::vector<tiny_dnn::label_t> test_labels;
  std::vector<tiny_dnn::vec_t> test_images;
  std::vector<unsigned int> times;
  parse_cifar10(src_filename + "/test_batch.bin", &test_images, &test_labels,-1.0, 1.0, 0, 0);
  int successes = 0;
  auto start = get_time::now(); 
  for(unsigned int i = 0; i < numtest; ++i) {
    auto inner_b = get_time::now();
    auto res = nn.predict(test_images[i]);
    auto inner_e = get_time::now();
    std::vector<std::pair<double, int>> scores;
    for (int i = 0; i < 10; i++) {
        scores.emplace_back(res[i], i);
    }
    sort(scores.begin(), scores.end(), std::greater<std::pair<double, int>>());
    if(test_labels[i] == scores[0].second ||
       test_labels[i] == scores[1].second || 
       test_labels[i] == scores[2].second ||
       test_labels[i] == scores[3].second ||
       test_labels[i] == scores[4].second) successes++;
    std::cout << "Inferencing (" << scores[0].second << "-"<< test_labels[i]   << ")" << i+1 << "/" << test_images.size() << "(Acc: " << (double)successes/double(i+1) << ")" << std::endl;
    std::clog << "\r" << i+1 << "/" << test_images.size();
    unsigned int tttt = std::chrono::duration_cast<ms>(inner_e - inner_b).count();
    times.push_back(tttt);
  }
  auto end = get_time::now();
  auto diff = end - start;
  make_test_report(test_images,successes,times,std::chrono::duration_cast<ms>(diff).count(),double(std::chrono::duration_cast<ms>(diff).count())/double(numtest),"../../../plot/cf_test_perf_");
  std::cout << "Accuracy: "  << successes << "/" << test_images.size() << "\n";
  std::cout << "Time elapsed: " << std::chrono::duration_cast<ms>(diff).count() << "ms\n";
}

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "please specify data path";
    return 0;
  }
  recognize(std::string(argv[1]), argv[2],atoi(argv[3]));
}
