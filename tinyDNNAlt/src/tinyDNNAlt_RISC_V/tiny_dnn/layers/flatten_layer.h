/*
    Copyright (c) 2013, Taiga Nomi and the respective contributors
    All rights reserved.

    Use of this source code is governed by a BSD-style license that can be found
    in the LICENSE file.
*/
#pragma once

#include <algorithm>
#include <string>
#include <vector>

#include "tiny_dnn/layers/layer.h"
#include "tiny_dnn/util/util.h"

namespace tiny_dnn {

class flatten_layer : public layer {
 public:
  /**
   * @param size_t width [in] input width
   * @param size_t height     [in] input heigh
   * @param size_t channels     [in] input channels
   */
  flatten_layer(size_t width,size_t height, size_t channels)
    : layer({vector_type::data}, {vector_type::data}),w_(width),h_(height),ch_(channels) {
      std::cout << w_ << " " << h_ << " " << ch_ << "\n";
  }

  std::string layer_type() const override { return "flatten"; }

  std::vector<shape3d> in_shape() const override { return {index3d<size_t>(w_, h_, ch_)}; }
  std::vector<shape3d> out_shape() const override { return {index3d<size_t>(w_*h_*ch_,1,1)}; }

  void forward_propagation(const std::vector<tensor_t *> &in_data,
                           std::vector<tensor_t *> &out_data) override {
      const size_t num_samples = (*out_data[0]).size();
      for_i(num_samples, [&](size_t s) {
          float_t *outs = &(*out_data[0])[s][0];
          const float_t *ins = &(*in_data[0])[s][0];
          outs = std::copy(ins, ins + w_*h_*ch_, outs);
      });
  }

  void back_propagation(const std::vector<tensor_t *> &in_data,
                        const std::vector<tensor_t *> &out_data,
                        std::vector<tensor_t *> &out_grad,
                        std::vector<tensor_t *> &in_grad) override {
  	    const size_t num_samples = (*out_data[0]).size();
		for(int i = 0; i < num_samples; ++i ) {
      		tiny_dnn::float_t *ins = &(*in_data[0])[i][0];
      		tiny_dnn::float_t *outs = &(*out_data[0])[i][0];
      		std::copy(outs,outs+w_*h_*ch_,ins);
		}
  }

  friend struct serialization_buddy;

 private:
  size_t w_,h_,ch_;
};

}  // namespace tiny_dnn
