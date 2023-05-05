/*
    Copyright (c) 2013, Taiga Nomi and the respective contributors
    All rights reserved.

    Use of this source code is governed by a BSD-style license that can be found
    in the LICENSE file.
*/
#pragma once

#include <vector>
#include "tiny_dnn/core/kernels/conv2d_op_internal.h"
#include "tiny_dnn/core/params/conv_params.h"




namespace tiny_dnn {
namespace kernels {
#ifdef CNN_USE_RVV

template <typename Allocator>
void rvv_conv2d_3x3_kernel(const core::conv_params &params,
                           const std::vector<tiny_dnn::float_t, Allocator> &in,
                           const std::vector<tiny_dnn::float_t, Allocator> &W,
                           const std::vector<tiny_dnn::float_t, Allocator> &bias,
                           std::vector<tiny_dnn::float_t, Allocator> &a,
                           const bool layer_parallelize) {
  CNN_UNREFERENCED_PARAMETER(layer_parallelize);
  assert(params.weight.height_ == 3 && params.weight.width_ == 3);

  auto &out       = params.out;
  auto &in_padded = params.in_padded;
  auto &tbl       = params.tbl;
  auto w_stride   = params.w_stride;

  const size_t out_area = out.area();
  size_t oidx           = 0;
  float bias_scale      = params.has_bias ? 1.0f : 0.0f;
  const size_t stride   = params.h_stride * in_padded.width_;
  const size_t inarea   = in_padded.area();


    for (size_t o = 0; o < out.depth_; ++o, oidx += out_area) {
      tiny_dnn::float_t *pa = &a[oidx];

      for (size_t inc = 0; inc < params.in.depth_; ++inc) {
        if (!tbl.is_connected(o, inc)) continue;

        const tiny_dnn::float_t *pw = (const tiny_dnn::float_t *)&W[9 * (params.in.depth_ * o + inc)];
        const tiny_dnn::float_t *pi = (const tiny_dnn::float_t *)&in[in_padded.get_index(0, 0, inc)];
        // actual conv
        pp_rvv.rvv_conv3x3(pi,pw,pa,in_padded.width_,in_padded.height_);
      }  
    }    // out depth loop    // else
}  // avx_conv2d_5x5_kernel float ver
// float ver
template <typename Allocator>
void rvv_conv2d_5x5_kernel(const core::conv_params &params,
                           const std::vector<tiny_dnn::float_t, Allocator> &in,
                           const std::vector<tiny_dnn::float_t, Allocator> &W,
                           const std::vector<tiny_dnn::float_t, Allocator> &bias,
                           std::vector<tiny_dnn::float_t, Allocator> &a,
                           const bool layer_parallelize) {
  CNN_UNREFERENCED_PARAMETER(layer_parallelize);
  assert(params.weight.height_ == 5 && params.weight.width_ == 5);

  auto &out       = params.out;
  auto &in_padded = params.in_padded;
  auto &tbl       = params.tbl;
  auto w_stride   = params.w_stride;

  const size_t out_area = out.area();
  size_t oidx           = 0;
  float bias_scale      = params.has_bias ? 1.0f : 0.0f;
  const size_t stride   = params.h_stride * in_padded.width_;
  const size_t inarea   = in_padded.area();


    for (size_t o = 0; o < out.depth_; ++o, oidx += out_area) {
      tiny_dnn::float_t *pa = &a[oidx];

      for (size_t inc = 0; inc < params.in.depth_; ++inc) {
        if (!tbl.is_connected(o, inc)) continue;

        const tiny_dnn::float_t *pw = (const tiny_dnn::float_t *)&W[25 * (params.in.depth_ * o + inc)];
        const tiny_dnn::float_t *pi = (const tiny_dnn::float_t *)&in[in_padded.get_index(0, 0, inc)];
        // actual conv

        //exit(0);
        pp_rvv.rvv_conv5x5(pi,pw,pa,in_padded.width_,in_padded.height_);
      }  
    }    // out depth loop    // else
}  // avx_conv2d_5x5_kernel float ver
#endif
inline void conv2d_op_rvv(const tensor_t &in_data,
                          const vec_t &W,
                          const vec_t &bias,
                          tensor_t &out_data,
                          const core::conv_params &params,
                          const bool layer_parallelize) {
#ifdef CNN_USE_RVV
  if (params.weight.height_ == 5 && params.weight.width_ == 5) {
    // @todo consider better parallelization
    for_i(layer_parallelize, in_data.size(), [&](size_t i) {
      rvv_conv2d_5x5_kernel(params, in_data[i], W, bias, out_data[i],
                            layer_parallelize);
    });
    return;
  } else if (params.weight.height_ == 3 && params.weight.width_ == 3) {
    for_i(layer_parallelize, in_data.size(), [&](size_t i) {
      rvv_conv2d_3x3_kernel(params, in_data[i], W, bias, out_data[i],
                            layer_parallelize);
    });
    return;
  }
#endif
  conv2d_op_internal(in_data, W, bias, out_data, params, layer_parallelize);
}

}  // namespace kernels
}  // namespace tiny_dnn
