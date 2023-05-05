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
template <size_t img_width,size_t img_height,size_t n_classes>
class alexnet : public tiny_dnn::network<tiny_dnn::sequential> {
 private:
    size_t getConvOutputSize(size_t input,size_t stride,size_t f_input) {
        float output = (float(input)-float(f_input))/float(stride) + 1;
        //std::cout << "[CONV " << f_input << "," << stride << "] In: " << input << "x" << input << " Out:" << output << "x" << output << std::endl;        
        return std::floor(output);
    }

    size_t getConvOutputSize(size_t input,size_t stride,size_t f_input,size_t& s) {
        std::cout << "Layer: " << s << std::endl;
        float output = (float(input)-float(f_input))/float(stride) + 1;
        s = std::floor(output);
        //std::cout << "[CONV " << f_input << "," << stride << "] In: " << input << "x" << input << " Out:" << s << "x" << s << std::endl;
        return s;
    }    
 public:
  explicit alexnet(bool include_classifier=false)
    : tiny_dnn::network<tiny_dnn::sequential>() {
    // todo: (karandesai) shift this to tiny_dnn::activation
    using relu     = tiny_dnn::activation::relu;
    using conv     = tiny_dnn::layers::conv;
    using fc     = tiny_dnn::layers::fc;
    using max_pool = tiny_dnn::layers::max_pool;
    size_t s;
    *this << conv(img_width, img_height, 10, 10, 3, 64, padding::valid, true, 4, 4);
    getConvOutputSize(img_width,4,11,s);
    *this << relu(s, s, 64);
    *this << max_pool(s, s, 64, 2);
    *this << conv(s/2, s/2, 5, 5, 64, 192, padding::valid, true, 1, 1);
    getConvOutputSize(s/2,1,5,s);
    *this << relu(s, s, 192);
    *this << max_pool(s, s, 192, 1);
    *this << conv(s, s, 3, 3, 192, 384, padding::valid, true, 1, 1);
    getConvOutputSize(s,1,3,s);
    *this << relu(s, s, 384);
    *this << conv(s, s, 3, 3, 384, 256, padding::valid, true, 1, 1);
    getConvOutputSize(s,1,3,s);
    *this << relu(s, s, 256);
    *this << conv(s, s, 3, 3, 256, 256, padding::valid, true, 1, 1);
    getConvOutputSize(s,1,3,s);
    *this << relu(s, s, 256);
    *this << max_pool(s, s, 256, 1);
    *this << conv(s,s,s,256,2048,padding::valid, true, 1, 1);
    *this << relu();
    std::cout << "Classifier...\n";
    if(!include_classifier) return;
    *this << fc(2048,2048,true);
    *this << relu();
    *this << fc(2048,n_classes,true);
  }


};


