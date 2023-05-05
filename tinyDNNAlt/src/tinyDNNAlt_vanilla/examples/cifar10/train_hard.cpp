/*
    Copyright (c) 2013, Taiga Nomi and the respective contributors
    All rights reserved.

    Use of this source code is governed by a BSD-style license that can be found
    in the LICENSE file.
*/
#include <cstdlib>
#include <iostream>
#include <vector>

#include "tiny_dnn/tiny_dnn.h"
template <typename N>
void construct_net(N &nn, tiny_dnn::core::backend_t backend_type) {
    using fc = tiny_dnn::layers::fc;
    using conv = tiny_dnn::layers::conv;
    using max_pool = tiny_dnn::layers::max_pool;
    using elu = tiny_dnn::activation::elu;
    using tiny_dnn::core::connection_table;
    using padding = tiny_dnn::padding;
    using softmax = tiny_dnn::activation::softmax;

    // VGG-16 Like network

    // Block 1
    nn << conv(32, 32, 3, 3, 3, 64, padding::same) << elu();
    nn << conv(32, 32, 3, 3, 64, 64, padding::same) << elu();
    nn << max_pool(32, 32, 64, 2) << elu();

    // Block 2
    nn << conv(16, 16, 3, 3, 64, 128, padding::same) << elu();
    nn << conv(16, 16, 3, 3, 128, 128, padding::same) << elu();
    nn << max_pool(16, 16, 128, 2) << elu();

    // Block 3
    nn << conv(8, 8, 3, 3, 128, 256, padding::same) << elu();
    nn << conv(8, 8, 3, 3, 256, 256, padding::same) << elu();
    nn << conv(8, 8, 3, 3, 256, 256, padding::same) << elu();
    nn << max_pool(8, 8, 256, 2) << elu();
    nn << fc(4096,10) << elu() << softmax();
}

template <typename N>
void construct_featnet(N &nn, tiny_dnn::core::backend_t backend_type) {
    using fc = tiny_dnn::layers::fc;
    using conv = tiny_dnn::layers::conv;
    using max_pool = tiny_dnn::layers::max_pool;
    using elu = tiny_dnn::activation::elu;
    using tiny_dnn::core::connection_table;
    using padding = tiny_dnn::padding;
    using softmax = tiny_dnn::activation::softmax;
    using dropout = tiny_dnn::layers::dropout;
    using gavepool = tiny_dnn::global_average_pooling_layer;
    // Efficientnet-like

    // Block 1
    nn << fc(1280,100) << softmax();
}

void train_cifar10(std::string data_dir_path,
                   double learning_rate,
                   const int n_train_epochs,
                   const int n_minibatch,
                   tiny_dnn::core::backend_t backend_type,
                   std::ostream &log,int num_train) {
  // specify loss-function and learning strategy
  tiny_dnn::network<tiny_dnn::sequential> nn;
  tiny_dnn::adagrad optimizer;

  construct_featnet(nn, backend_type);

  std::cout << "load models..." << std::endl;

  // load cifar dataset
  std::vector<tiny_dnn::label_t> train_labels_o, test_labels_o;
  std::vector<tiny_dnn::vec_t> train_images_o, test_images_o;

  for (int i = 1; i <= 5; i++) {
    //parse_cifar10(data_dir_path + "/data_batch_" + std::to_string(i) + ".bin",
     //             &train_images_o, &train_labels_o, -1.0, 1.0, 0, 0);
  }
  //parse_cifar100(data_dir_path+"/train.bin",&train_images_o, &train_labels_o, -1.0, 1.0, 0, 0);

  parse_binImagenet_features<1280,1,1,uint16_t>(data_dir_path + "/cifar_100_feat.bin", &train_images_o, &train_labels_o,
                -1.0, 1.0);
  parse_binImagenet_features<1280,1,1,uint16_t>(data_dir_path + "/cifar_100_test_features.bin", &test_images_o, &test_labels_o,
                -1.0, 1.0);                

  std::cout << "start learning: " << train_labels_o.size() << std::endl;

  /*SUBSAMPLING*/
  std::vector<tiny_dnn::label_t> train_labels(test_labels_o.begin(),test_labels_o.begin()+num_train);
  std::vector<tiny_dnn::vec_t> train_images(test_images_o.begin(),test_images_o.begin()+num_train);
  std::vector<tiny_dnn::label_t> test_labels(test_labels_o.begin(),test_labels_o.begin()+1000);
  std::vector<tiny_dnn::vec_t> test_images(test_images_o.begin(),test_images_o.begin()+1000);
  /****************/

  tiny_dnn::progress_display disp(train_images.size());
  tiny_dnn::timer t;

optimizer.alpha *=
    std::min(tiny_dnn::float_t(4),
             static_cast<tiny_dnn::float_t>(sqrt(n_minibatch) * learning_rate));  int epoch = 1;
  // create callback
  auto on_enumerate_epoch = [&]() {
    std::cout << "Epoch " << epoch << "/" << n_train_epochs << " finished. "
              << t.elapsed() << "s elapsed." << std::endl;
    ++epoch;
    tiny_dnn::result res = nn.test(train_images, train_labels);
    std::cout << res.accuracy() << "%" << std::endl;

    disp.restart(train_images.size());
    t.restart();
  };

  auto on_enumerate_minibatch = [&]() { disp += n_minibatch; };

  // training
  nn.train<tiny_dnn::cross_entropy>(optimizer, train_images, train_labels,
                                    n_minibatch, n_train_epochs,
                                    on_enumerate_minibatch, on_enumerate_epoch);

  std::cout << "end training." << std::endl;

  // test and show results
  nn.test(test_images, test_labels).print_detail(std::cout);
  // save networks
  nn.save("effnet_cf100.model");
}

static tiny_dnn::core::backend_t parse_backend_name(const std::string &name) {
  const std::array<const std::string, 5> names = {
    "internal", "nnpack", "libdnn", "avx", "opencl",
  };
  for (size_t i = 0; i < names.size(); ++i) {
    if (name.compare(names[i]) == 0) {
      return static_cast<tiny_dnn::core::backend_t>(i);
    }
  }
  return tiny_dnn::core::default_engine();
}

static void usage(const char *argv0) {
  std::cout << "Usage: " << argv0 << " --data_path path_to_dataset_folder"
            << " --learning_rate 0.01"
            << " --epochs 30"
            << " --minibatch_size 10"
            << " --backend_type internal" << std::endl;
}

int main(int argc, char **argv) {
  unsigned int seed = 132145;
  tiny_dnn::set_random_seed(seed);
  double learning_rate                   = 0.01;
  int epochs                             = 30;
  std::string data_path                  = "";
  int minibatch_size                     = 10;
  int num_train                          = 50000;
  tiny_dnn::core::backend_t backend_type = tiny_dnn::core::default_engine();

  if (argc == 2) {
    std::string argname(argv[1]);
    if (argname == "--help" || argname == "-h") {
      usage(argv[0]);
      return 0;
    }
  }
  for (int count = 1; count + 1 < argc; count += 2) {
    std::string argname(argv[count]);
    if (argname == "--learning_rate") {
      learning_rate = atof(argv[count + 1]);
    } else if (argname == "--epochs") {
      epochs = atoi(argv[count + 1]);
    } else if (argname == "--minibatch_size") {
      minibatch_size = atoi(argv[count + 1]);
    } else if (argname == "--backend_type") {
      backend_type = parse_backend_name(argv[count + 1]);
    } else if (argname == "--data_path") {
      data_path = std::string(argv[count + 1]);
    } else if (argname == "--num_train") {
      num_train = atoi(argv[count+1]);
    } else {
      std::cerr << "Invalid parameter specified - \"" << argname << "\""
                << std::endl;
      usage(argv[0]);
      return -1;
    }
  }
  if (data_path == "") {
    std::cerr << "Data path not specified." << std::endl;
    usage(argv[0]);
    return -1;
  }
  if (learning_rate <= 0) {
    std::cerr
      << "Invalid learning rate. The learning rate must be greater than 0."
      << std::endl;
    return -1;
  }
  if (epochs <= 0) {
    std::cerr << "Invalid number of epochs. The number of epochs must be "
                 "greater than 0."
              << std::endl;
    return -1;
  }
  if (minibatch_size <= 0 || minibatch_size > 50000) {
    std::cerr
      << "Invalid minibatch size. The minibatch size must be greater than 0"
         " and less than dataset size (50000)."
      << std::endl;
    return -1;
  }
  std::cout << "Running with the following parameters:" << std::endl
            << "Data path: " << data_path << std::endl
            << "Learning rate: " << learning_rate << std::endl
            << "Minibatch size: " << minibatch_size << std::endl
            << "Number of epochs: " << epochs << std::endl
            << "Backend type: " << backend_type << std::endl
            << std::endl;
  try {
    train_cifar10(data_path, learning_rate, epochs, minibatch_size,
                  backend_type, std::cout,num_train);
  } catch (tiny_dnn::nn_error &err) {
    std::cerr << "Exception: " << err.what() << std::endl;
  }
}
