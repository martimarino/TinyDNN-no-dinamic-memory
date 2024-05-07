/*
    Copyright (c) 2013, Taiga Nomi and the respective contributors
    All rights reserved.

    Use of this source code is governed by a BSD-style license that can be found
    in the LICENSE file.
*/
#pragma once
#include <vector>
#include "tiny_dnn/core/backend.h"
#include "tiny_dnn/core/framework/device.fwd.h"

namespace tiny_dnn {

/**
 * Abstract class for recurrent cells.
 **/
class cell : public layer {
 public:
  cell() : layer({}, {}) {}

  virtual etl::vector<vector_type, MAX_INPUT_SIZE> input_order() = 0;

  virtual etl::vector<vector_type, MAX_OUTPUT_SIZE> output_order() = 0;

  virtual void forward_propagation(const etl::vector<tensor_t *, MAX_TENSOR_SIZE> &in_data,
                                   etl::vector<tensor_t *, MAX_TENSOR_SIZE> &out_data) = 0;

  virtual void back_propagation(const etl::vector<tensor_t *, MAX_TENSOR_SIZE> &in_data,
                                const etl::vector<tensor_t *, MAX_TENSOR_SIZE> &out_data,
                                etl::vector<tensor_t *, MAX_TENSOR_SIZE> &out_grad,
                                etl::vector<tensor_t *, MAX_TENSOR_SIZE> &in_grad) = 0;

  virtual core::backend_t backend_type() const { return wrapper_->engine(); }

  virtual void init_backend(const layer *wrapper) = 0;

 protected:
  inline void set_wrapper(const layer *wrapper) { wrapper_ = wrapper; }

  const layer *wrapper_;  // every forward iteration, we must get the engine,
                          // backend, etc from the wrapper
};

}  // namespace tiny_dnn
