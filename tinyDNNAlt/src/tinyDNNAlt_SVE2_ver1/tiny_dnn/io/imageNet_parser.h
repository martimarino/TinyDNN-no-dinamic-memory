#pragma once
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>
#include "tiny_dnn/util/util.h"

namespace tiny_dnn {

template <size_t width,size_t height,size_t depth,class LabelType> 
inline void parse_binImagenet(const std::string &filename,
                          std::vector<vec_t> *train_images,
                          std::vector<label_t> *train_labels,
                          float_t scale_min,
                          float_t scale_max) {
  

  if (scale_min >= scale_max)
    throw nn_error("scale_max must be greater than scale_min");

  std::ifstream ifs(filename.c_str(), std::ios::in | std::ios::binary);
  if (ifs.fail() || ifs.bad())
    throw nn_error("failed to open file:" + filename);
  LabelType label;
  std::vector<unsigned char> buf(width*height*depth);

  while (ifs.read(reinterpret_cast<char *>(&label), sizeof(LabelType))) {
    vec_t img;
    if (!ifs.read(reinterpret_cast<char *>(&buf[0]), width*height*depth)) break;
      std::transform(buf.begin(), buf.end(), std::back_inserter(img),
                     [=](unsigned char c) {
                       return scale_min + (scale_max - scale_min) * float_t(double(c) / 255);
                     });
    train_images->push_back(img);
    train_labels->push_back(label);
  }
}

} 
