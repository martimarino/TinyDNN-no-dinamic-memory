/*
    Copyright (c) 2019, Federico Rossi and the respective contributors
    All rights reserved.

    Use of this source code is governed by a BSD-style license that can be found
    in the LICENSE file.
*/
#pragma once

#include <string>



// Based on:
// https://github.com/DeepMark/deepmark/blob/master/torch/image%2Bvideo/alexnet.lua
template <size_t width,size_t height,size_t n_classes>
class resnet152 : public tiny_dnn::network<tiny_dnn::sequential> {
 public:
  explicit resnet152(bool include_classifier=false)
    : tiny_dnn::network<tiny_dnn::sequential>() {
    // todo: (karandesai) shift this to tiny_dnn::activation
    using relu     = tiny_dnn::activation::relu;
    using conv     = tiny_dnn::layers::conv;
    using fc     = tiny_dnn::layers::fc;
    using max_pool = tiny_dnn::layers::max_pool;
    using ave_pool = tiny_dnn::global_average_pooling_layer;
    using sotfmax = tiny_dnn::activation::softmax;

    size_t img_width = width,img_height = height;
    *this << conv(img_width,img_height,7,7,3,64,padding::same,true, 2, 2, 2, 2) << relu();
    *this << max_pool(img_width=img_width/2,img_height=img_height/2,64,3,3,2,2,false,padding::same);
    *this << tiny_dnn::residual::make_block3(img_width=img_width/2,img_height=img_height/2,64,false,true);
    *this << tiny_dnn::residual::make_block3(img_width,img_height,64,false);
    *this << tiny_dnn::residual::make_block3(img_width,img_height,64,true);
    
    *this << tiny_dnn::residual::make_block3(img_width=img_width/2,img_height=img_height/2,128,false);
    for(int i = 0; i < 6; ++i)
	*this << tiny_dnn::residual::make_block3(img_width,img_height,128,false);
    *this << tiny_dnn::residual::make_block3(img_width,img_height,128,true);

    *this << tiny_dnn::residual::make_block3(img_width=img_width/2,img_height=img_height/2,256,false);
    for(int i = 0; i < 34;++i)
	*this << tiny_dnn::residual::make_block3(img_width,img_height,256,false);
    *this << tiny_dnn::residual::make_block3(img_width,img_height,256,true);

    *this << tiny_dnn::residual::make_block3(img_width=img_width/2,img_height=img_height/2,512,false);
    *this << tiny_dnn::residual::make_block3(img_width,img_height,512,false);
    *this << tiny_dnn::residual::make_block3(img_width,img_height,512,false);

    if(!include_classifier) return;
    *this << ave_pool(img_width,img_height,4*512) << fc(4*512,1000) << sotfmax();


  }


};


