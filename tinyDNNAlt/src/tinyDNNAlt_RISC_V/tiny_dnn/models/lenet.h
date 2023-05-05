/*
    Copyright (c) 2013, Taiga Nomi and the respective contributors
    All rights reserved.

    Use of this source code is governed by a BSD-style license that can be found
    in the LICENSE file.
*/
#pragma once

#include <string>



// Based on:
// https://github.com/DeepMark/deepmark/blob/master/torch/image%2Bvideo/alexnet.lua
class lenet : public tiny_dnn::network<tiny_dnn::sequential> {
 private:
  
 public:
  explicit lenet()
    : tiny_dnn::network<tiny_dnn::sequential>() {
    // todo: (karandesai) shift this to tiny_dnn::activation
    using fc = tiny_dnn::layers::fc;
    using conv = tiny_dnn::layers::conv;
    using max_pool = tiny_dnn::layers::max_pool;
    using tanh = tiny_dnn::activation::elu;
    using sigmoid = tiny_dnn::activation::sigmoid;
    using padding = tiny_dnn::padding;
    using smax = tiny_dnn::activation::softmax;
    using globalpool = tiny_dnn::global_average_pooling_layer;
    *this << conv(32, 32, 3, 1, 32,   // C1, 1@32x32-in, 32@32x32-out
               padding::same, true, 1, 1, 1, 1)
       << tanh()
       << max_pool(32, 32, 32, 2)   // S2, 32@32x32-in, 32@16x16-out
       << conv(16, 16, 3, 32, 32,   // C2, 32@16x16-in, 32@16x16-out
               padding::same, true, 1, 1, 1, 1)
       << tanh()
       << max_pool(16, 16, 32, 2)   // S2, 32@16x16-in, 32@8x8-out
       << conv(8, 8, 3, 32, 32,   // C3, 32@8x8-in, 32@8x8-out
               padding::same, true, 1, 1, 1, 1)
       << tanh()
       << max_pool(8, 8, 32, 2)   // S2, 32@8x8-in, 32@4x4-out       
       << conv(4, 4, 3, 32, 32,   // C4, 32@4x4-in, 32@4x4-out
               padding::same, true, 1, 1, 1, 1)
       << tanh()
       << max_pool(4, 4, 32, 2)   // S2, 32@4x4-in, 32@2x2-out        
       << conv(2, 2, 3, 32, 32,   // C4, 32@2x2-in, 32@2x2-out
               padding::same, true, 1, 1, 1, 1)
       << tanh()
       << max_pool(2, 2, 32, 2)   // S2, 32@2x2-in, 32@1x1-out       
       << fc(32,10,true)
       << smax();
  }


};


