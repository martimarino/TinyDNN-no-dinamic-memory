/*
    Copyright (c) 2013, Taiga Nomi and the respective contributors
    All rights reserved.

    Use of this source code is governed by a BSD-style license that can be found
    in the LICENSE file.
*/
#pragma once

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

#include "tiny_dnn/util/util.h"

#define CIFAR100_IMAGE_DEPTH (3)
#define CIFAR100_IMAGE_WIDTH (32)
#define CIFAR100_IMAGE_HEIGHT (32)
#define CIFAR100_IMAGE_AREA (CIFAR100_IMAGE_WIDTH * CIFAR100_IMAGE_HEIGHT)
#define CIFAR100_IMAGE_SIZE (CIFAR100_IMAGE_AREA * CIFAR100_IMAGE_DEPTH)

namespace tiny_dnn {

/**
 * parse CIFAR-100 database format images
 *
 * @param filename [in] filename of database(binary version)
 * @param train_images [out] parsed images
 * @param train_labels [out] parsed labels
 * @param scale_min  [in]  min-value of output
 * @param scale_max  [in]  max-value of output
 * @param x_padding  [in]  adding border width (left,right)
 * @param y_padding  [in]  adding border width (top,bottom)
 **/
inline void parse_cifar100(const std::string &filename,
                          std::vector<vec_t> *train_images,
                          std::vector<label_t> *train_labels,
                          float_t scale_min,
                          float_t scale_max,
                          int x_padding,
                          int y_padding) {
  if (x_padding < 0 || y_padding < 0)
    throw nn_error("padding size must not be negative");
  if (scale_min >= scale_max)
    throw nn_error("scale_max must be greater than scale_min");

  std::ifstream ifs(filename.c_str(), std::ios::in | std::ios::binary);
  if (ifs.fail() || ifs.bad())
    throw nn_error("failed to open file:" + filename);

  uint8_t label,garbage;
  std::vector<unsigned char> buf(CIFAR100_IMAGE_SIZE);

  while (ifs.read(reinterpret_cast<char *>(&label), 1)) {
    // first read of label is the coarse one, not interested
    // read again
    ifs.read(reinterpret_cast<char *>(&label), 1);
    vec_t img;
    if (!ifs.read(reinterpret_cast<char *>(&buf[0]), CIFAR100_IMAGE_SIZE)) break;

    if (x_padding || y_padding) {
      int w = CIFAR100_IMAGE_WIDTH + 2 * x_padding;
      int h = CIFAR100_IMAGE_HEIGHT + 2 * y_padding;

      img.resize(w * h * CIFAR100_IMAGE_DEPTH, scale_min);

      for (int c = 0; c < CIFAR100_IMAGE_DEPTH; c++) {
        for (int y = 0; y < CIFAR100_IMAGE_HEIGHT; y++) {
          for (int x = 0; x < CIFAR100_IMAGE_WIDTH; x++) {
            img[c * w * h + (y + y_padding) * w + x + x_padding] =
              scale_min +
              (scale_max - scale_min) *
                float_t(double(buf[c * CIFAR100_IMAGE_AREA + y * CIFAR100_IMAGE_WIDTH + x] / 255));
          }
        }
      }
    } else {
      std::transform(buf.begin(), buf.end(), std::back_inserter(img),
                     [=](unsigned char c) {
                       return scale_min + (scale_max - scale_min) * float_t(double(c) / 255);
                     });
    }

    train_images->push_back(img);
    train_labels->push_back(label);
  }
}

}  // namespace tiny_dnn
