/*
    Copyright (c) 2013, Taiga Nomi and the respective contributors
    All rights reserved.

    Use of this source code is governed by a BSD-style license that can be found
    in the LICENSE file.
*/
#pragma once

#include <vector>

#include "tiny_dnn/core/kernels/fully_connected_op_internal.h"


namespace tiny_dnn {
namespace kernels {
#ifdef CNN_USE_RVV

template <typename Allocator>
inline void rvv_fully_connected_forward_kernel(
  const std::vector<std::vector<tiny_dnn::float_t, Allocator>> &in_data,
  const std::vector<tiny_dnn::float_t, Allocator> &W,
  const std::vector<tiny_dnn::float_t, Allocator> &bias,
  std::vector<std::vector<tiny_dnn::float_t, Allocator>> &out_data,
  const core::fully_params &params,
  const bool layer_parallelize) {
    for_i(layer_parallelize, in_data.size(), [&](size_t sample) {
      const auto &in = in_data[sample];
      auto &out      = out_data[sample];
      pp_rvv.rvvGemmX(W,in,out,params.out_size_,params.in_size_,1);

      /*for (size_t i = 0; i < params.out_size_; i++) {
        float sum = 0.0f;
        for (size_t c = 0; c < params.in_size_; c++) {
          sum += W[c * params.out_size_ + i] * in[c];
        }
        out[i] = sum;*/
      }
    );
  }
  inline void fully_connected_op_rvv(const tensor_t &in_data,
                                   const vec_t &W,
                                   const vec_t &bias,
                                   tensor_t &out_data,
                                   const core::fully_params &params,
                                   const bool layer_parallelize) {
  rvv_fully_connected_forward_kernel(in_data, W, bias, out_data, params,
                                     layer_parallelize);
}

/*template <typename Allocator>
inline void avx_fully_connected_forward_kernel(
  const std::vector<std::vector<double, Allocator>> &in_data,
  const std::vector<double, Allocator> &W,
  const std::vector<double, Allocator> &bias,
  std::vector<std::vector<double, Allocator>> &out_data,
  const core::fully_params &params,
  const bool layer_parallelize) {
  // fallback to tiny-backend when float_t is double
  fully_connected_op_internal(in_data, W, bias, out_data, params,
                              layer_parallelize);
}

template <typename Allocator>
inline void avx_fully_connected_back_kernel(
  const std::vector<std::vector<float, Allocator>> &prev_out,
  const std::vector<float, Allocator> &W,
  std::vector<std::vector<float, Allocator>> &dW,
  std::vector<std::vector<float, Allocator>> &db,
  std::vector<std::vector<float, Allocator>> &curr_delta,
  std::vector<std::vector<float, Allocator>> &prev_delta,
  const core::fully_params &params,
  const bool layer_parallelize) {
  if (params.has_bias_) {
    for (size_t sample = 0; sample < prev_out.size(); sample++) {
      auto &prev_delta2 = prev_delta[sample];
      auto &curr_delta2 = curr_delta[sample];
      auto &prev_out2   = prev_out[sample];
      auto &dW2         = dW[sample];
      auto &db2         = db[sample];
      for (size_t c = 0; c < params.in_size_; c++) {
        // propagate delta to previous layer
        // prev_delta[c] += current_delta[r] * W_[c * out_size_ + r]
        prev_delta2[c] += vectorize::dot(
          &curr_delta2[0], &W[c * params.out_size_], params.out_size_);
      }
      for_(layer_parallelize, 0u, params.out_size_,
           [&](const blocked_range &r) {
             // accumulate weight-step using delta
             // dW[c * out_size + i] += current_delta[i] * prev_out[c]
             size_t len = r.end() - r.begin();
             for (size_t c = 0; c < params.in_size_; c++) {
               vectorize::muladd(&curr_delta2[r.begin()], prev_out2[c], len,
                                 &dW2[c * params.out_size_ + r.begin()]);
             }
             // vec_t& db = *in_grad[2];
             vectorize::reduce(&curr_delta2[r.begin()], len, &db2[r.begin()]);
           });
    }
  } else {
    for (size_t sample = 0; sample < prev_out.size(); sample++) {
      auto &prev_delta2 = prev_delta[sample];
      auto &curr_delta2 = curr_delta[sample];
      auto &prev_out2   = prev_out[sample];
      auto &dW2         = dW[sample];
      for (size_t c = 0; c < params.in_size_; c++) {
        // propagate delta to previous layer
        // prev_delta[c] += current_delta[r] * W_[c * out_size_ + r]
        prev_delta2[c] += vectorize::dot(
          &curr_delta2[0], &W[c * params.out_size_], params.out_size_);
      }
      for_(layer_parallelize, 0u, params.out_size_,
           [&](const blocked_range &r) {
             // accumulate weight-step using delta
             // dW[c * out_size + i] += current_delta[i] * prev_out[c]
             size_t len = r.end() - r.begin();
             for (size_t c = 0; c < params.in_size_; c++) {
               vectorize::muladd(&curr_delta2[r.begin()], prev_out2[c], len,
                                 &dW2[c * params.out_size_ + r.begin()]);
             }
           });
    }
  }
}

template <typename Allocator>
inline void avx_fully_connected_back_kernel(
  const std::vector<std::vector<double, Allocator>> &prev_out,
  const std::vector<double, Allocator> &W,
  std::vector<std::vector<double, Allocator>> &dW,
  std::vector<std::vector<double, Allocator>> &db,
  std::vector<std::vector<double, Allocator>> &curr_delta,
  std::vector<std::vector<double, Allocator>> &prev_delta,
  const core::fully_params &params,
  const bool layer_parallelize) {
  // fallback to tiny-backend when float_t is double
  fully_connected_op_internal(prev_out, W, dW, db, curr_delta, prev_delta,
                              params, layer_parallelize);
}*/



#endif  // CNN_USE_AVX



/*inline void fully_connected_op_avx(const tensor_t &prev_out,
                                   const vec_t &W,
                                   tensor_t &dW,
                                   tensor_t &db,
                                   tensor_t &curr_delta,
                                   tensor_t &prev_delta,
                                   const core::fully_params &params,
                                   const bool layer_parallelize) {
#ifdef CNN_USE_RVV
  avx_fully_connected_back_kernel(prev_out, W, dW, db, curr_delta, prev_delta,
                                  params, layer_parallelize);
#else
  CNN_UNREFERENCED_PARAMETER(prev_out);
  CNN_UNREFERENCED_PARAMETER(W);
  CNN_UNREFERENCED_PARAMETER(dW);
  CNN_UNREFERENCED_PARAMETER(db);
  CNN_UNREFERENCED_PARAMETER(curr_delta);
  CNN_UNREFERENCED_PARAMETER(prev_delta);
  CNN_UNREFERENCED_PARAMETER(params);
  CNN_UNREFERENCED_PARAMETER(layer_parallelize);
  throw nn_error("TinyDNN has not been compiled with AVX support.");
#endif
}*/

}  // namespace kernels
}  // namespace tiny_dnn
