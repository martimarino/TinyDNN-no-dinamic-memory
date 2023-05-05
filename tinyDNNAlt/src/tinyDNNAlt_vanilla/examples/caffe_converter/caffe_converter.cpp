#include <ctime>
#include <iostream>
#include <memory>

#define NO_STRICT
#define CNN_USE_CAFFE_CONVERTER

#ifndef DNN_USE_IMAGE_API
#define DNN_USE_IMAGE_API
#endif

#include "tiny_dnn/tiny_dnn.h"
#include "tiny_dnn/util/util.h"


void test(const std::string &model_file,
          const std::string &trained_file,
          const std::string &out_file) {
  auto net    = tiny_dnn::create_net_from_caffe_prototxt(model_file);
  tiny_dnn::reload_weight_from_caffe_protobinary(trained_file, net.get());
  std::cout << "Saving network to tiny model " << out_file << std::endl;
  (*net).save(out_file);
}

void test(const std::string &model_file,
          const std::string &trained_file,
          const std::string &out_file,
          const tiny_dnn::shape3d& input_shape) {
  auto net    = tiny_dnn::create_net_from_caffe_prototxt(model_file,input_shape);
  std::cout << "Done..." << std::endl;
  tiny_dnn::reload_weight_from_caffe_protobinary(trained_file, net.get());
  std::cout << "============================================================" << std::endl;
  auto nn = *net;
  for (int i = 0; i < nn.depth(); i++) {
    std::cout << "#layer:" << i << "\n";
    std::cout << "layer type:" << nn[i]->layer_type() << "\n";
    std::cout << "input:" << nn[i]->in_data_size() << "(" << nn[i]->in_data_shape() << ")\n";
    std::cout << "output:" << nn[i]->out_data_size() << "(" << nn[i]->out_data_shape() << ")\n";
}
  std::cout << "Saving network to tiny model " << out_file << std::endl;
  (*net).save(out_file);
}

int main(int argc, char **argv) {
  int arg_channel          = 1;
  if(argc < 4) {
    std::cout << "Usage: caffe_converter .prototxt .caffemodel out_tiny_model" << std::endl;
    std::cout << "[OPTIONAL] Usage: caffe_converter .prototxt .caffemodel out_tiny_model [input_width input_heigth input_depth]" << std::endl;

    exit(1);
  }
  bool with_shape = false;
  int width,height,depth;
  std::string model_file   = argv[arg_channel++];
  std::string trained_file = argv[arg_channel++];
  std::string tiny_model_out_file = argv[arg_channel++];
  std::cout << tiny_model_out_file << std::endl;
  if(argc == 7) {
    with_shape = true;
    width = atoi(argv[arg_channel++]);
    height = atoi(argv[arg_channel++]);
    depth = atoi(argv[arg_channel++]);
  }
  try {
    if(!with_shape)
      test(model_file, trained_file,tiny_model_out_file);
    else
      test(model_file,trained_file,tiny_model_out_file,tiny_dnn::shape3d(width,height,depth));
  } catch (const tiny_dnn::nn_error &e) {
    std::cout << e.what() << std::endl;
  }
}
