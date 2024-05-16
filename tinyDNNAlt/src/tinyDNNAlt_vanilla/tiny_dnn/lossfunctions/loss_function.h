/*
    Copyright (c) 2013, Taiga Nomi and the respective contributors
    All rights reserved.

    Use of this source code is governed by a BSD-style license that can be found
    in the LICENSE file.
*/
#pragma once

#include <vector>

#include "tiny_dnn/util/util.h"

namespace tiny_dnn {

// mean-squared-error loss function for regression
class mse {
 public:
  static float_t f(const vec_t &y, const vec_t &t) {
    assert(y.size() == t.size());
    float_t d{0.0f};
    for (size_t i = 0; i < y.size(); ++i) {
      //std::cout << y[i] << " " << t[i] << std::endl;
      //std::cout << y[i] - t[i] << std::endl;
      d += (y[i] - t[i]) * (y[i] - t[i]);
    }
  	return d/float_t(double(y.size()));

  }

  static vec_t df(const vec_t &y, const vec_t &t) {
    assert(y.size() == t.size());
    vec_t d(t.size());

	//WORKAROUND__B
	//double d_factor = 2 / t.size();
	//float_t factor = static_cast<float_t>(d_factor);

	float_t factor = float_t(2) / float_t(double((t.size())));
	//WORKAROUND__E

    for (size_t i = 0; i < y.size(); ++i) {
      d[i] = factor * (y[i] - t[i]);
    }
    return d;
  }
};

// absolute loss function for regression
class absolute {
 public:
  static float_t f(const vec_t &y, const vec_t &t) {
    assert(y.size() == t.size());
    float_t d{0};

    for (size_t i = 0; i < y.size(); ++i) d += std::abs(y[i] - t[i]);
	//WORKAROUND__B

	return d / float_t(double(y.size()));
    //return d / static_cast<float_t>(y.size());
	//WORKAROUND__E
  }

  static vec_t df(const vec_t &y, const vec_t &t) {
    assert(y.size() == t.size());
    vec_t d(t.size());
	//WORKAROUND__B


	float_t factor = float_t(1) / float_t(double(t.size()));
	//WORKAROUND__E
    for (size_t i = 0; i < y.size(); ++i) {
      float_t sign = y[i] - t[i];
      if (sign < float_t{0.f})
        d[i] = -factor;
      else if (sign > float_t{0.f})
        d[i] = factor;
      else
        d[i] = {0.0};
    }

    return d;
  }
};

// absolute loss with epsilon range for regression
// epsilon range [-eps, eps] with eps = 1./fraction
template <int fraction>
class absolute_eps {
 public:
  static float_t f(const vec_t &y, const vec_t &t) {
    assert(y.size() == t.size());
    float_t d{0};
    const float_t eps = float_t(1) / float_t(double(fraction));

    for (size_t i = 0; i < y.size(); ++i) {
      float_t diff = std::abs(y[i] - t[i]);
      if (diff > eps) d += diff;
    }
    return d / float_t(double(y.size()));
  }

  static vec_t df(const vec_t &y, const vec_t &t) {
    assert(y.size() == t.size());
    vec_t d(t.size());
    const float_t factor = float_t(1) / float_t(double(t.size()));
    const float_t eps    = float_t(1) / float_t(double(fraction));

    for (size_t i = 0; i < y.size(); ++i) {
      float_t sign = y[i] - t[i];
      if (sign < -eps)
        d[i] = -factor;
      else if (sign > eps)
        d[i] = factor;
      else
        d[i] = 0.f;
    }
    return d;
  }
};

// cross-entropy loss function for (multiple independent) binary classifications
class cross_entropy {
 public:
  static float_t f(const vec_t &y, const vec_t &t) {
    assert(y.size() == t.size());
    float_t d{0};
	
    for (size_t i = 0; i < y.size(); ++i)
      d += -t[i] * tiny_dnn::log(y[i]) -
           (float_t(1) - t[i]) * tiny_dnn::log(float_t(1) - y[i]);

    return d;
  }

  static vec_t df(const vec_t &y, const vec_t &t) {
    assert(y.size() == t.size());
    vec_t d(t.size());

    for (size_t i = 0; i < y.size(); ++i)
      d[i]        = (y[i] - t[i]) / (y[i] * (float_t(1) - y[i]));

    return d;
  }
};

// cross-entropy loss function for multi-class classification
class cross_entropy_multiclass {
 public:
  static float_t f(const vec_t &y, const vec_t &t) {
    assert(y.size() == t.size());
    float_t d{0.0};

    for (size_t i = 0; i < y.size(); ++i) d += -t[i] * tiny_dnn::log(y[i]);

    return d;
  }

  static vec_t df(const vec_t &y, const vec_t &t) {
    assert(y.size() == t.size());
    vec_t d(t.size());

    for (size_t i = 0; i < y.size(); ++i) d[i] = -t[i] / y[i];

    return d;
  }
};

template <typename E>
vec_t gradient(const vec_t &y, const vec_t &t) {
  assert(y.size() == t.size());
  return E::df(y, t);
}

template <typename E>
<<<<<<< HEAD
etl::vector<vec_t, MAX_TENSOR_SIZE> gradient(const etl::vector<vec_t, MAX_TENSOR_SIZE> &y,
                            const etl::vector<vec_t, MAX_TENSOR_SIZE> &t) {
  etl::vector<vec_t, MAX_TENSOR_SIZE> grads(y.size());
=======
etl::vector<vec_t, 1> gradient(const etl::vector<vec_t, MAX_TENSOR_SIZE> &y,
                            const etl::vector<vec_t, MAX_TENSOR_SIZE> &t) {
  etl::vector<vec_t, 1> grads(y.size());
>>>>>>> b4fac29b69f002784c4991180fcd8db997312c1c

  assert(y.size() == t.size());

  for (size_t i = 0; i < y.size(); i++) grads[i] = gradient<E>(y[i], t[i]);

  return grads;
}

inline void apply_cost_if_defined(etl::vector<vec_t, MAX_TENSOR_SIZE> &sample_gradient,
                                  const etl::vector<vec_t, MAX_TENSOR_SIZE> &sample_cost) {
  if (sample_gradient.size() == sample_cost.size()) {
    // @todo consider adding parallelism
    const size_t channel_count = sample_gradient.size();
    for (size_t channel = 0; channel < channel_count; ++channel) {
      if (sample_gradient[channel].size() == sample_cost[channel].size()) {
        const size_t element_count = sample_gradient[channel].size();

        // @todo optimize? (use AVX or so)
        for (size_t element = 0; element < element_count; ++element) {
          sample_gradient[channel][element] *= sample_cost[channel][element];
        }
      }
    }
  }
}

// gradient for a minibatch
template <typename E>
etl::vector<tensor_t, 1> gradient(const etl::vector<tensor_t, MAX_OUTPUT_SIZE> &y,
                               const etl::vector<tensor_t, MAX_OUTPUT_SIZE> &t,
                               const etl::vector<tensor_t, MAX_OUTPUT_SIZE> &t_cost) {
  const size_t sample_count  = y.size();
  const size_t channel_count = y[0].size();

  etl::vector<tensor_t, 1> gradients(sample_count);

  CNN_UNREFERENCED_PARAMETER(channel_count);
  assert(y.size() == t.size());
  assert(t_cost.empty() || t_cost.size() == t.size());

  // @todo add parallelism
  for (size_t sample = 0; sample < sample_count; ++sample) {
    assert(y[sample].size() == channel_count);
    assert(t[sample].size() == channel_count);
    assert(t_cost.empty() || t_cost[sample].empty() ||
           t_cost[sample].size() == channel_count);

    gradients[sample] = gradient<E>(y[sample], t[sample]);

    if (sample < t_cost.size()) {
      apply_cost_if_defined(gradients[sample], t_cost[sample]);
    }
  }

  return gradients;
}

}  // namespace tiny_dnn
