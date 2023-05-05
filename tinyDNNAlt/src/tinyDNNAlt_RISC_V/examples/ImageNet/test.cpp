#include <iostream>
# include <chrono>

//#define CNN_USE_POSIT
#include "tiny_dnn/tiny_dnn.h"

using  ns = std::chrono::nanoseconds;
using  ms = std::chrono::milliseconds;
using get_time = std::chrono::steady_clock ;
using namespace tiny_dnn;

static void gtrsbnet(tiny_dnn::network<tiny_dnn::sequential>& nn) {
	#define O true
	#define X false
	static const bool tbl[] = {
	    O, X, X, X, O, O, O, X, X, O, O, O, O, X, O, O,
	    O, O, X, X, X, O, O, O, X, X, O, O, O, O, X, O,
	    O, O, O, X, X, X, O, O, O, X, X, O, X, O, O, O,
	    X, O, O, O, X, X, O, O, O, O, X, X, O, X, O, O,
	    X, X, O, O, O, X, X, O, O, O, O, X, O, O, X, O,
	    X, X, X, O, O, O, X, X, O, O, O, O, X, O, O, O
	};
	#undef O
	#undef X

    using fc = tiny_dnn::layers::fc;
    using conv = tiny_dnn::layers::conv;
    using ave_pool = tiny_dnn::layers::ave_pool;
    using elu = tiny_dnn::activation::elu;
    using smax = tiny_dnn::activation::softmax;
    using tiny_dnn::core::connection_table;
    using padding = tiny_dnn::padding;

    nn << conv(32, 32, 5, 1, 6,   // C1, 1@32x32-in, 6@28x28-out
               padding::valid, true, 1, 1, 1, 1)
       << elu()
       << ave_pool(28, 28, 6, 2)   // S2, 6@28x28-in, 6@14x14-out
       << elu()
       << conv(14, 14, 5, 6, 16,   // C3, 6@14x14-in, 16@10x10-out
               connection_table(tbl, 6, 16),
               padding::valid, true, 1, 1, 1, 1)
       << elu()
       << ave_pool(10, 10, 16, 2)  // S4, 16@10x10-in, 16@5x5-out
       << elu()
       << conv(5, 5, 5, 16, 120,   // C5, 16@5x5-in, 120@1x1-out
               padding::valid, true, 1, 1, 1, 1)
       << elu()
       << fc(120, 43, true)  // F6, 120-in, 10-out
       << smax();
}

 void test_time(tiny_dnn::network<tiny_dnn::sequential> &net,size_t width,size_t height,int runs) {
    image<> tmp(shape3d(width,height,1),image_type::grayscale);

    for(int i = 0; i < runs; ++i) {
      auto start = get_time::now();
      std::cout << "Predicting...\n";
      //net.predict(tmp);
      net.load("test8");
      auto end = get_time::now();
      std::cout << std::chrono::duration_cast<ns>(end - start).count() << std::endl;
    }
}

int main(int argc, char **argv) {
  int runs = atoi(argv[1]);
  std::cout << "LeNet..." << std::endl;
    using conv = tiny_dnn::layers::conv;
        using elu = tiny_dnn::activation::elu;
    using ave_pool = tiny_dnn::layers::ave_pool;
    using fc = tiny_dnn::layers::fc;
    using smax = tiny_dnn::activation::softmax;


   tiny_dnn::network<tiny_dnn::sequential> net; 
   gtrsbnet(net);
 // tiny_dnn::models::alexnet<224,224,1000> net;
//  tiny_dnn::models::vgg16<1000> net;
  test_time(net,32,32,runs);
}
