/*
    Copyright (c) 2017, Taiga Nomi
    All rights reserved.

    Use of this source code is governed by a BSD-style license that can be found
    in the LICENSE file.
*/
#pragma once

namespace tiny_dnn {
namespace kernels {

#ifdef CNN_USE_RVV

// float version
inline void global_avepool_op_rvv(const tensor_t &in_data,
                                  tensor_t &out_data,
                                  const core::global_avepool_params &params,
                                  const bool layer_parallelize) {
  const size_t pool_area            = params.in.width_ * params.in.height_;

  for_i(layer_parallelize, in_data.size(), [&](size_t sample) {
    const vec_t &in = in_data[sample];
    vec_t &out      = out_data[sample];
    for (size_t i = 0; i < params.in.depth_; i++) {
      const tiny_dnn::float_t *pin = &in[i * pool_area];
      std::cout << "RVV avepool" << std::endl;
      pp_rvv.rvv_avepool2x2(pin,out,params.in.height_,params.in.width_);
    }
  });
}


#endif  // RVV
}  // namespace kernels
}  // namespace tiny_dnn
