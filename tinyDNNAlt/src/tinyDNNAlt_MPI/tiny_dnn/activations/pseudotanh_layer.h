/*
    Copyright (c) 2013, Taiga Nomi and the respective contributors
    All rights reserved.

    Use of this source code is governed by a BSD-style license that can be found
    in the LICENSE file.
*/
#pragma once

#include <string>
#include <utility>

#include "tiny_dnn/activations/activation_layer.h"
#include "tiny_dnn/layers/layer.h"

namespace tiny_dnn {

class pseudotanh_layer : public activation_layer {
 public:
  using activation_layer::activation_layer;

  std::string layer_type() const override { return "pseudotanh-activation"; }
  // y[j] = 2*sigmoid(2*x) - 1;
  void forward_activation(const vec_t &x, vec_t &y) override {
    std::cout << "Pseudotanh\n";
    for (size_t j = 0; j < x.size(); j++) {
      y[j] = tiny_dnn::pseudotanh(x[j]);
    }
  }

  void backward_activation(const vec_t &x,
                           const vec_t &y,
                           vec_t &dx,
                           const vec_t &dy) override {
    for (size_t j = 0; j < x.size(); j++) {
      // dx = dy * 4*(gradient of sigmoid(2y))
      //dx[j] = 4*dy[j] * 2*y[j] * (float_t(1) - 2*y[j]);
      dx[j] = dy[j] * (float_t(1) - sqr(y[j]));      
    }
  }

  std::pair<float_t, float_t> scale() const override { //NOT VALID FOR PSEUDOTANH
    return std::make_pair(float_t(0.1), float_t(0.9));
  }

  friend struct serialization_buddy;
};

}  // namespace tiny_dnn
