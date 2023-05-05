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



void recognize(const std::string &net, const std::string &src_filename,int numtest,float min,float max) {
  //startLog(false);
  tiny_dnn::network<tiny_dnn::sequential> nn;
  nn.load(net);
  
  //endLog("cifar_network_weights.txt");
  /*====================================================*/

  std::vector<tiny_dnn::label_t> test_labels;
  std::vector<tiny_dnn::vec_t> test_images;
  std::vector<unsigned int> times;
  parse_binImagenet_features<1280,1,1,uint16_t>(src_filename + "/cifar_100_test_features.bin", &test_images, &test_labels,
                (tiny_dnn::float_t)-1.0,(tiny_dnn::float_t) 1.0);  int successes = 0;
  auto start = get_time::now(); 
  for(unsigned int i = 0; i < numtest; ++i) {
    auto inner_b = get_time::now();
    auto res = nn.predict(test_images[i]);
    auto inner_e = get_time::now();
    std::vector<std::pair<double, int>> scores;
    for (int j = 0; j < 100; j++) {
        scores.emplace_back(res[j], j);
    }
    sort(scores.begin(), scores.end(), std::greater<std::pair<double, int>>());
    if(test_labels[i] == scores[0].second /*|| //top-5
       test_labels[i] == scores[1].second ||
       test_labels[i] == scores[2].second ||
       test_labels[i] == scores[3].second ||
       test_labels[i] == scores[4].second*/ ) successes++;
    std::cout << "Inferencing (" << scores[0].second << "-"<< test_labels[i]   << ")" << i+1 << "/" << test_images.size() << "(Acc: " << (double)successes/double(i+1) << ")" << std::endl;
    //std::clog << "\r" << i+1 << "/" << test_images.size();
    unsigned int tttt = std::chrono::duration_cast<ms>(inner_e - inner_b).count();
    std::cout << tttt << " ms\n";
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
  recognize(std::string(argv[1]), argv[2],atoi(argv[3]),atof(argv[4]),atof(argv[5]));
}


/*

CIFAR100 top-5:
              %      ms
Softfloat32 90.2%   ~CIFAR-10
Posit16     90.2%   ~CIFAR-10
Posit14     90.2%   ~CIFAR-10
Posit12     90.2%   ~CIFAR-10
Posit10     90.2%   ~CIFAR-10
Posit8      80%     ~CIFAR-10


*/