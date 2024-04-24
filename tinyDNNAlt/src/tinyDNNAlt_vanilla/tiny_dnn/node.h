/*
    Copyright (c) 2013, Taiga Nomi and the respective contributors
    All rights reserved.

    Use of this source code is governed by a BSD-style license that can be found
    in the LICENSE file.
*/
#pragma once

#include <algorithm>
#include <iomanip>
#include <memory>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <unordered_set>
#include <vector>
#include "etl/vector.h"
#include "etl/memory.h"

#include "tiny_dnn/optimizers/optimizer.h"
#include "tiny_dnn/util/product.h"
#include "tiny_dnn/util/util.h"
#include "tiny_dnn/util/weight_init.h"

#ifdef DNN_USE_IMAGE_API
#include "tiny_dnn/util/image.h"
#endif

#include "tiny_dnn/util/parameters.h"

namespace tiny_dnn {

    class node;
    class layer;
    class edge;
    typedef std::shared_ptr<edge> edgeptr_t;

/**
 * base class of all kind of tinny-cnn data
 **/
    class node : public std::enable_shared_from_this<node> {
    public:
        node(size_t in_size, size_t out_size) : prev_(in_size), next_(out_size) {prev_.resize(in_size); next_.resize(out_size);}
        virtual ~node() {}

        const etl::vector<edgeptr_t, MAX_NODES> &prev() const { return prev_; }
        const etl::vector<edgeptr_t, MAX_NODES> &next() const { return next_; }

        size_t prev_port(const edge &e) const {
            size_t index = 0;
            for (const auto &ep : prev_) {  //se si trova un elemento che corrisponda a quello cercato restituisce l'indice in cui e' stato trovato
                if (ep.get() == &e) {
                    return index;
                }
                ++index;
            }
            return etl::size(prev_); //se l'elemento cercato non e' stato trovato si restituisce la dimensione del vector
        }

        size_t next_port(const edge &e) const {
            size_t index = 0;
            for (const auto &ep : next_) {
                if (ep.get() == &e) {   //se si trova un elemento che corrisponda a quello cercato restituisce l'indice in cui e' stato trovato
                    return index;
                }
                ++index;
            }
            return etl::size(next_); //se l'elemento cercato non e' stato trovato si restituisce la dimensione del vector
        }

        etl::vector<node *, MAX_NODES> prev_nodes()
        const;  // @todo refactor and remove this method
        etl::vector<node *, MAX_NODES> next_nodes()
        const;  // @todo refactor and remove this method

    protected:
        node() = delete;

        friend void connect(layer *head,
                            layer *tail,
                            size_t head_index,
                            size_t tail_index);

        mutable etl::vector<edgeptr_t, MAX_NODES> prev_;
        mutable etl::vector<edgeptr_t, MAX_NODES> next_;
    };

/**
 * class containing input/output data
 **/
    class edge {
    public:
        edge(node *prev, const shape3d &shape, vector_type vtype)
                : shape_(shape),
                  vtype_(vtype),
                  data_({vec_t(shape.size())}),
                  grad_({vec_t(shape.size())}),
                  prev_(prev) {}

        void merge_grads(vec_t *dst) {
            assert(!grad_.empty());
            const auto &grad_head = grad_[0];
            size_t sz             = grad_head.size();
            dst->resize(sz);
            float_t *pdst = etl::addressof((*dst)[0]);
            // dst = grad_[0]
            etl::copy(grad_head.begin(), grad_head.end(), pdst);
            // @todo consider adding parallelism
            for (size_t sample = 1, sample_count = grad_.size(); sample < sample_count;
                 ++sample) {
                // dst += grad_[sample]
                vectorize::reduce(etl::addressof(grad_[sample][0]), sz, pdst);
            }
        }

        void clear_grads() {
            for (size_t sample = 0, sample_count = grad_.size(); sample < sample_count;
                 ++sample) {
                auto &g = grad_[sample];
                vectorize::fill(etl::addressof(g[0]), g.size(), float_t{0});
            }
        }

        tensor_t *get_data() { return etl::addressof(data_); }

        const tensor_t *get_data() const { return etl::addressof(data_); }

        tensor_t *get_gradient() { return etl::addressof(grad_); }

        const tensor_t *get_gradient() const { return etl::addressof(grad_); }

        const etl::vector<node *, MAX_NODES> &next() const { return next_; }
        node *prev() { return prev_; }
        const node *prev() const { return prev_; }

        const shape3d &shape() const { return shape_; }
        vector_type vtype() const { return vtype_; }
        void add_next_node(node *next) { next_.push_back(next); }

    private:
        shape3d shape_;
        vector_type vtype_;
        tensor_t data_;
        tensor_t grad_;
        node *prev_;                // previous node, "producer" of this tensor
        etl::vector<node *, MAX_NODES> next_;  // next nodes, "consumers" of this tensor
    };

    inline etl::vector<node *, MAX_NODES> node::prev_nodes() const {
        etl::vector<node *,MAX_NODES> vecs;
        for (auto &e : prev_) {
           if (e && e->prev()) {
             vecs.insert(vecs.end(), e->prev());
           }
         }
         return vecs;
    }

    inline etl::vector<node *, MAX_NODES> node::next_nodes() const {
        etl::vector<node *, MAX_NODES> vecs;
        for (auto &e : next_) {
            if (e) {
              auto n = e->next();
              vecs.insert(vecs.end(), n.begin(), n.end());
            }
          }
          return vecs;
    }

    template <typename T>
    struct layer_tuple {
        layer_tuple(T l1, T l2) {
            layers_.push_back(l1);
            layers_.push_back(l2);
        }
        etl::vector<T, 2> layers_;
    };

    template <
            typename T,
            typename U,
            typename std::enable_if<std::is_base_of<layer, T>::value &&
                                    std::is_base_of<layer, U>::value>::type * = nullptr>
    layer_tuple<layer *> operator,(T &l1, U &l2) {
        return layer_tuple<layer *>(&l1, &l2);
    }

    template <
            typename T,
            typename U,
            typename std::enable_if<std::is_base_of<layer, T>::value &&
                                    std::is_base_of<layer, U>::value>::type * = nullptr>
    layer_tuple<std::shared_ptr<layer>> operator,(std::shared_ptr<T> l1,
    std::shared_ptr<U> l2) {
    return layer_tuple<std::shared_ptr<layer>>(l1, l2);
}

template <
        typename T,
        typename std::enable_if<std::is_base_of<layer, T>::value>::type * = nullptr>
layer_tuple<layer *> operator,(layer_tuple<layer *> lhs, T &rhs) {
    lhs.layers_.push_back(&rhs);
    return lhs;
}

template <
        typename T,
        typename std::enable_if<std::is_base_of<layer, T>::value>::type * = nullptr>
layer_tuple<std::shared_ptr<layer>> operator,(
        layer_tuple<std::shared_ptr<layer>> lhs, std::shared_ptr<T> &rhs) {
    lhs.layers_.push_back(rhs);
    return lhs;
}

template <
        typename T,
        typename std::enable_if<std::is_base_of<layer, T>::value>::type * = nullptr>
layer_tuple<layer *> operator,(T &lhs, layer_tuple<layer *> rhs) {
    rhs.layers_.insert(rhs.layers_.begin(), &lhs);
    return rhs;
}

template <
        typename T,
        typename std::enable_if<std::is_base_of<layer, T>::value>::type * = nullptr>
layer_tuple<std::shared_ptr<layer>> operator,(
        std::shared_ptr<T> &lhs, layer_tuple<std::shared_ptr<layer>> rhs) {
    rhs.layers_.insert(rhs.layers_.begin(), lhs);
    return rhs;
}

template <typename T, typename U>
inline std::shared_ptr<U> &operator<<(std::shared_ptr<T> &lhs,
                                      std::shared_ptr<U> &rhs) {
    connect(lhs.get(), rhs.get());
    return rhs;
}

template <typename T>
inline T &operator<<(const layer_tuple<std::shared_ptr<layer>> &lhs, T &rhs) {
    for (size_t i = 0; i < lhs.layers_.size(); i++) {
        connect(&*lhs.layers_[i], &*rhs, 0, i);
    }
    return rhs;
}

template <typename T>
inline const layer_tuple<std::shared_ptr<layer>> &operator<<(
        T &lhs, const layer_tuple<std::shared_ptr<layer>> &rhs) {
    for (size_t i = 0; i < rhs.layers_.size(); i++) {
        connect(&*lhs, &*rhs.layers_[i], i, 0);
    }
    return rhs;
}

template <typename T>
inline T &operator<<(const layer_tuple<layer *> &lhs, T &rhs) {
    for (size_t i = 0; i < lhs.layers_.size(); i++) {
        connect(lhs.layers_[i], &rhs, 0, i);
    }
    return rhs;
}

template <typename T>
inline const layer_tuple<layer *> &operator<<(T &lhs,
                                              const layer_tuple<layer *> &rhs) {
    for (size_t i = 0; i < rhs.layers_.size(); i++) {
        connect(&lhs, rhs.layers_[i], i, 0);
    }
    return rhs;
}
}  // namespace tiny_dnn
