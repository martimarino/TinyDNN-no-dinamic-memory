#pragma once

#include <string>
#include <utility>

#include "tiny_dnn/activations/activation_layer.h"
#include "tiny_dnn/layers/layer.h"

namespace tiny_dnn {
enum ShortcutSide { BEGIN, END };
thread_local std::vector<std::queue<vec_t>*> shortcuts_;
class shortcut_layer : public activation_layer {
 private:
  unsigned int mapping_;
  ShortcutSide side_;
 public:
  static unsigned int shortcut_counter_;
  using activation_layer::activation_layer;

  shortcut_layer(ShortcutSide side,size_t w,size_t h,size_t c):activation_layer(shape3d(w, h, c)) {
    mapping_ = shortcut_counter_ = shortcuts_.size();
    side_ = side;
    if(side == END)
      shortcuts_.push_back(new std::queue<vec_t>());
  }
  std::string layer_type() const override { return "shortcut-"+std::to_string(mapping_); }

  void forward_activation(const vec_t &x, vec_t &y) override {
    if(side_ == BEGIN)
      shortcuts_.at(mapping_)->push(vec_t(x.begin(),x.end()));
    for (size_t j = 0; j < x.size(); j++) {
        y[j] = x[j];
      if(side_ == END) {
        y[j] += float_t(0);/*shortcuts_.at(mapping_)->front().at(j);*/
        //shortcuts_.at(mapping_)->pop();
      }
    }
  }

  void backward_activation(const vec_t &x,
                           const vec_t &y,
                           vec_t &dx,
                           const vec_t &dy) override {
    for (size_t j = 0; j < x.size(); j++) {
      dx[j] = dy[j];
    }
  }

  std::pair<float_t, float_t> scale() const override {
    return std::make_pair(float_t(-0.8), float_t(0.8));
  }

  friend struct serialization_buddy;
};
unsigned int shortcut_layer::shortcut_counter_ = 0;

}  // namespace tiny_dnn
