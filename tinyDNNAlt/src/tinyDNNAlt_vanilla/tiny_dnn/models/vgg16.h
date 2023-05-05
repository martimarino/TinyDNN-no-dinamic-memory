/*
    Copyright (c) 2019, Federico Rossi and the respective contributors
    All rights reserved.

    Use of this source code is governed by a BSD-style license that can be found
    in the LICENSE file.
*/
#pragma once

#include <string>



template <size_t n_classes>
class vgg16 : public tiny_dnn::network<tiny_dnn::sequential> {
 public:
  explicit vgg16(bool include_classifier=true)
    : tiny_dnn::network<tiny_dnn::sequential>() {
    // todo: (karandesai) shift this to tiny_dnn::activation
    using relu     = tiny_dnn::activation::relu;
    using conv     = tiny_dnn::layers::conv;
    using fc     = tiny_dnn::layers::fc;
    using max_pool = tiny_dnn::layers::max_pool;
    using sotfmax = tiny_dnn::activation::softmax;


    // BLOCK 1
    *this << conv(224, 224, 3, 3, 3, 64, padding::same);
    *this << relu();
    *this << conv(224, 224, 3, 3, 64, 64, padding::same);
    *this << relu();
    *this << max_pool(224, 224, 64, 2);

    // BLOCK 2
    *this << conv(112, 112, 3, 3, 64, 128, padding::same);
    *this << relu();
    *this << conv(112, 112, 3, 3, 128, 128, padding::same);
    *this << relu();
    *this << max_pool(112, 112, 128, 2);

    // BLOCK 3
    *this << conv(56, 56, 3, 3, 128, 256, padding::same);
    *this << relu();
    *this << conv(56, 56, 3, 3, 256, 256, padding::same);
    *this << relu();
    *this << conv(56, 56, 3, 3, 256, 256, padding::same);
    *this << relu();
    *this << max_pool(56, 56, 256, 2);

    // BLOCK 4
    *this << conv(28, 28, 3, 3, 256, 512, padding::same);
    *this << relu();
    *this << conv(28, 28, 3, 3, 512, 512, padding::same);
    *this << relu();
    *this << conv(28, 28, 3, 3, 512, 512, padding::same);
    *this << relu();
    *this << max_pool(28, 28, 512, 2); 

    // BLOCK 5
    *this << conv(14, 14, 3, 3, 512, 512, padding::same);
    *this << relu();
    *this << conv(14, 14, 3, 3, 512, 512, padding::same);
    *this << relu();
    *this << conv(14, 14, 3, 3, 512, 512, padding::same);
    *this << relu();
    *this << max_pool(14, 14, 512, 2);  

    // FULLY CONNECTED
    if(!include_classifier) return;
    *this << fc(7*7*512,4096) << relu();
    *this << fc(4096,4096) << relu();
    *this << fc(4096,n_classes) << sotfmax();

  }


};


