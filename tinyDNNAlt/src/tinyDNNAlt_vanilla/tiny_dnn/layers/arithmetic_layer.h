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

/**
 * element-wise add N vectors ```y_i = x0_i + x1_i + ... + xnum_i```
 **/
class elementwise_add_layer : public layer {
 public:
  /**
   * @param num_args [in] number of inputs
   * @param dim      [in] number of elements for each input
   */
  elementwise_add_layer(size_t num_args, size_t dim)
    : layer(etl::vector<vector_type, MAX_INPUT_SIZE>(num_args, vector_type::data),
            {vector_type::data}),
      num_args_(num_args),
      dim_(dim) {}

  std::string layer_type() const override { return "elementwise-add"; }

  etl::vector<shape3d, MAX_TENSOR_SIZE> in_shape() const override {
    return etl::vector<shape3d, MAX_TENSOR_SIZE>(num_args_, shape3d(dim_, 1, 1));
  }

  etl::vector<shape3d, MAX_TENSOR_SIZE> out_shape() const override {
    return {shape3d(dim_, 1, 1)};
  }

  void forward_propagation(const etl::vector<tensor_t *, MAX_TENSOR_SIZE> &in_data,
                           etl::vector<tensor_t *, MAX_TENSOR_SIZE> &out_data) override {
    const tensor_t &in1 = *in_data[0];
    tensor_t &out       = *out_data[0];

    out = in1;

    // @todo parallelize
    for (size_t sample = 0; sample < in1.size(); ++sample) {
      for (size_t i = 1; i < num_args_; i++) {
        std::transform((*in_data[i])[sample].begin(),
                       (*in_data[i])[sample].end(), out[sample].begin(),
                       out[sample].begin(),
                       [](float_t x, float_t y) { return x + y; });
      }
    }
  }

  void back_propagation(const etl::vector<tensor_t *, MAX_TENSOR_SIZE> &in_data,
                        const etl::vector<tensor_t *, MAX_TENSOR_SIZE> &out_data,
                        etl::vector<tensor_t *, MAX_TENSOR_SIZE> &out_grad,
                        etl::vector<tensor_t *, MAX_TENSOR_SIZE> &in_grad) override {
    CNN_UNREFERENCED_PARAMETER(in_data);
    CNN_UNREFERENCED_PARAMETER(out_data);
    for (size_t i = 0; i < num_args_; i++) *in_grad[i] = *out_grad[0];
  }

  friend struct serialization_buddy;

 private:
  size_t num_args_;
  size_t dim_;
};

}  // namespace tiny_dnn
