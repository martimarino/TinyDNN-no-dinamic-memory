#include <ctime>
#include <iostream>
#include <memory>

//#define QT_BOARD

#include "tiny_dnn/tiny_dnn.h"
#include "tiny_dnn/util/util.h"
#include "tiny_dnn/models/alexnet.h"
#include "tiny_dnn/models/NNTrainParams.h"
#include "tiny_dnn/models/NNModel.h"
using  ns = std::chrono::nanoseconds;
using  ms = std::chrono::milliseconds;
using get_time = std::chrono::steady_clock ;
using namespace tiny_dnn;

static tiny_dnn::network<tiny_dnn::sequential>* construct_net() {
	tiny_dnn::network<tiny_dnn::sequential>* nn = new tiny_dnn::network<tiny_dnn::sequential>();
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
    using tanh = tiny_dnn::activation::sigmoid;
    using smax = tiny_dnn::activation::softmax;
    using tiny_dnn::core::connection_table;
    using padding = tiny_dnn::padding;

    *nn << conv(32, 32, 5, 3, 6,   // C1, 1@32x32-in, 6@28x28-out
               padding::valid, true, 1, 1, 1, 1)
       << tanh()
       << ave_pool(28, 28, 6, 2)   // S2, 6@28x28-in, 6@14x14-out
       << tanh()
       << conv(14, 14, 5, 6, 16,   // C3, 6@14x14-in, 16@10x10-out
               connection_table(tbl, 6, 16),
               padding::valid, true, 1, 1, 1, 1)
       << tanh()
       << ave_pool(10, 10, 16, 2)  // S4, 16@10x10-in, 16@5x5-out
       << tanh()
       << conv(5, 5, 5, 16, 120,   // C5, 16@5x5-in, 120@1x1-out
               padding::valid, true, 1, 1, 1, 1)
       << tanh()
       << fc(120, 43, true)  // F6, 120-in, 10-out
       << smax();
       return nn;
}


int main(int argc, char **argv) {
  /*
    argv[1] -> train folder
    argv[2] -> test file
    argv[3] -> train params

  */
  //srand(time(NULL));
  unsigned int seed = rand();
  std::cout << "Random seed:\n" << seed << std::endl;
  tiny_dnn::set_random_seed(seed);

	std::vector<label_t> tlab,vlab;
	std::vector<vec_t> timg,vimg;
  std::string base_path(argv[1]);
  for(int i = 0; i < 42;++i) 
      parse_binImagenet<32,32,3,uint8_t>(base_path+"/"+std::to_string(i)+".bin",&timg,&tlab,-2,2);
  vutil::randomShuffle(timg,tlab);
  std::cout << "Num train images: " << timg.size() << std::endl;
  parse_binImagenet<32,32,3,uint8_t>(argv[2],&vimg,&vlab,-2,2);
  std::cout << "Num test images: " << vimg.size() << std::endl;

  tiny_dnn::adagrad optimizer;
  optimizer.alpha*=2;
  NNTrainParams tparams(argv[3],&optimizer,tlab,vlab,timg,vimg);
  std::cout << tparams;
	NNModel model("GTRSB-TANH",*construct_net());
  model.train(tparams);
  model.save();
}
