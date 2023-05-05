#include <iostream>
# include <chrono>

//#define CNN_USE_POSIT
#include "tiny_dnn/tiny_dnn.h"
#include "tiny_dnn/models/alexnet.h"
#include "tiny_dnn/models/vgg16.h"
#include "tiny_dnn/models/resnet34.h"
using  ns = std::chrono::nanoseconds;
using  ms = std::chrono::milliseconds;
using get_time = std::chrono::steady_clock ;
using namespace tiny_dnn;

double test_time(tiny_dnn::network<tiny_dnn::sequential>& net,size_t width,size_t height) {
    image<> tmp(shape3d(width,height,3),image_type::rgb);

    auto start = get_time::now();
    net.predict(tmp);
    auto end = get_time::now();
	

    return double(std::chrono::duration_cast<ms>(end - start).count());
}

int main(int argc, char **argv) {
  alexnet<224,224,1000> net;
  std::cout << "Elapsed (ms): " << test_time(net,224,224);
}
