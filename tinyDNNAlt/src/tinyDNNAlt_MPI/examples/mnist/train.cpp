#include <iostream>
#include <sstream>
#include <fstream>
#include "tiny_dnn/tiny_dnn.h"


/*
  REPORT_LEVEL
  0 -> Only final test+train sets evaluation
  1 -> Final evaluation + test set evaluation on each epoch
  2 -> Final evaluation + test and train set evaluation on each epoch
*/
int REPORT_LEVEL=1; 


static void construct_net(tiny_dnn::network<tiny_dnn::sequential> &nn,tiny_dnn::core::backend_t backend_type) {
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
    using max_pool = tiny_dnn::layers::max_pool;
    using tanh = tiny_dnn::activation::elu;
    using sigmoid = tiny_dnn::activation::sigmoid;
    using tiny_dnn::core::connection_table;
    using padding = tiny_dnn::padding;
    using smax = tiny_dnn::activation::softmax;
    using globalpool = tiny_dnn::global_average_pooling_layer;
    nn << conv(32, 32, 3, 1, 32,   // C1, 1@32x32-in, 32@32x32-out
               padding::same, true, 1, 1, 1, 1, backend_type)
       << tanh()
       << max_pool(32, 32, 32, 2)   // S2, 32@32x32-in, 32@16x16-out
       << conv(16, 16, 3, 32, 32,   // C2, 32@16x16-in, 32@16x16-out
               padding::same, true, 1, 1, 1, 1, backend_type)
       << tanh()
       << max_pool(16, 16, 32, 2)   // S2, 32@16x16-in, 32@8x8-out
       << conv(8, 8, 3, 32, 32,   // C3, 32@8x8-in, 32@8x8-out
               padding::same, true, 1, 1, 1, 1, backend_type)
       << tanh()
       << max_pool(8, 8, 32, 2)   // S2, 32@8x8-in, 32@4x4-out       
       << conv(4, 4, 3, 32, 32,   // C4, 32@4x4-in, 32@4x4-out
               padding::same, true, 1, 1, 1, 1, backend_type)
       << tanh()
       << max_pool(4, 4, 32, 2)   // S2, 32@4x4-in, 32@2x2-out        
       << conv(2, 2, 3, 32, 32,   // C4, 32@2x2-in, 32@2x2-out
               padding::same, true, 1, 1, 1, 1, backend_type)
       << tanh()
       << max_pool(2, 2, 32, 2)   // S2, 32@2x2-in, 32@1x1-out       
       << fc(32,10,true)
       << smax();

}



static void train_lenet(const std::string &data_dir_path,
                        double learning_rate,
                        const int n_train_epochs,
                        const int n_minibatch,
                        tiny_dnn::core::backend_t backend_type,const int num_train,std::string& resume_path,bool fashion = false) {
  // specify loss-function and learning strategy
  tiny_dnn::network<tiny_dnn::sequential> nn;

  tiny_dnn::adagrad optimizer;
  if(resume_path.length() == 0) {
    if(!fashion)
      construct_net(nn, backend_type);
    else
      construct_net(nn,backend_type);
  } else { //load partially trained network from file
    nn.load(resume_path);
  }

  std::cout << "load models..." << std::endl;

  // load MNIST dataset
  std::vector<tiny_dnn::label_t> train_o_labels, test_o_labels;
  std::vector<tiny_dnn::vec_t> train_o_images, test_o_images;

  tiny_dnn::parse_mnist_labels(data_dir_path + "/train-labels.idx1-ubyte",
                               &train_o_labels);
  tiny_dnn::parse_mnist_images(data_dir_path + "/train-images.idx3-ubyte",
                               &train_o_images, -1.0, 1.0, 2, 2);
  tiny_dnn::parse_mnist_labels(data_dir_path + "/t10k-labels.idx1-ubyte",
                               &test_o_labels);
  tiny_dnn::parse_mnist_images(data_dir_path + "/t10k-images.idx3-ubyte",
                               &test_o_images, -1.0, 1.0, 2, 2);

  std::vector<tiny_dnn::label_t> train_labels(train_o_labels.begin(),train_o_labels.begin()+num_train), 
                                 test_labels(test_o_labels.begin(),test_o_labels.begin()+100);
  std::vector<tiny_dnn::vec_t> train_images(train_o_images.begin(),train_o_images.begin()+num_train), 
                               test_images(test_o_images.begin(),test_o_images.begin()+100);
  std::cout << "start training" << std::endl;
  tiny_dnn::progress_display disp(train_images.size());
  tiny_dnn::timer t;

  optimizer.alpha *=
    std::min(tiny_dnn::float_t(4),
             static_cast<tiny_dnn::float_t>(sqrt(n_minibatch) * learning_rate));

  int epoch = 1;
  tiny_dnn::float_t total_elapsed = tiny_dnn::float_t{0.0};
  std::vector<unsigned int> test_succ;
  std::vector<unsigned int> train_succ;
  // create callback
  auto on_enumerate_epoch = [&]() {
    total_elapsed+=t.elapsed();
    std::cout << std::endl <<  "Epoch " << epoch << "/" << n_train_epochs << " finished. "
              << t.elapsed() << "s elapsed." << std::endl;
    ++epoch;
    if(REPORT_LEVEL > 0) {
      tiny_dnn::result res = nn.test(test_images, test_labels);
      test_succ.push_back(res.num_success);
      std::cout << "Test: " << res.num_success << "/" << res.num_total << std::endl;
      if(REPORT_LEVEL > 1) {
        tiny_dnn::result res_tr = nn.test(train_images, train_labels);
        train_succ.push_back(res_tr.num_success);
        std::cout << "Train: " << res_tr.num_success << "/" << res_tr.num_total << std::endl;
      }
    }

    disp.restart(train_images.size());
    t.restart();
  };

  auto on_enumerate_minibatch = [&]() { disp += n_minibatch;};

  // training
  nn.train<tiny_dnn::cross_entropy_multiclass>(optimizer, train_images, train_labels, n_minibatch,
                          n_train_epochs, on_enumerate_minibatch,
                          on_enumerate_epoch,false);

		if(REPORT_LEVEL>0) {
		make_train_report(test_succ,learning_rate,n_train_epochs,n_minibatch,num_train,"../../plot/epochsPerf");
		if(REPORT_LEVEL>1)
		  make_train_report(train_succ,test_succ,learning_rate,n_train_epochs,n_minibatch,num_train,"../../plot/epochsPerf");
		}
		std::cout << "end training. Total time: " << total_elapsed << std::endl;
		make_final(nn,learning_rate,n_train_epochs,n_minibatch,num_train,test_labels,train_labels,test_images,train_images,total_elapsed);
}

static tiny_dnn::core::backend_t parse_backend_name(const std::string &name) {
  const std::array<const std::string, 5> names = {{
    "internal", "nnpack", "libdnn", "avx", "opencl",
  }};
  for (size_t i = 0; i < names.size(); ++i) {
    if (name.compare(names[i]) == 0) {
      return static_cast<tiny_dnn::core::backend_t>(i);
    }
  }
  return tiny_dnn::core::default_engine();
}

static void usage(const char *argv0) {
  std::cout << "Usage: " << argv0 << " --data_path path_to_dataset_folder"
            << " --learning_rate 1"
            << " --epochs 30"
            << " --minibatch_size 16"
            << " --backend_type internal"
            << " --num_train 5000"
            << " --report_level 1"  
            << " --resume path_to_model\n";
}

int main(int argc, char **argv) {
  unsigned int seed = 132145;
  tiny_dnn::set_random_seed(seed);
  double learning_rate                   = 1 ;
  int epochs                             = 30;
  std::string data_path                  = "";
  int minibatch_size                     = 16;
  int num_train                          = 5000;
  bool fashion = false;
  std::string resume_path = "";
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
      num_train = atoi(argv[count + 1]);
    } else if (argname == "--report_level") {
      REPORT_LEVEL = atoi(argv[count + 1]);
    } else if (argname == "--fashion") {
      fashion = (atoi(argv[count+1]) == 1)?1:0;
    } else if (argname == "--resume") {
      resume_path = std::string(argv[count+1]);
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
  if (minibatch_size <= 0 || minibatch_size > 60000) {
    std::cerr
      << "Invalid minibatch size. The minibatch size must be greater than 0"
         " and less than dataset size (60000)."
      << std::endl;
    return -1;
  }
  std::cout << "Running with the following parameters:" << std::endl
            << "Data path: " << data_path << std::endl
            << "Learning rate: " << learning_rate << std::endl
            << "Minibatch size: " << minibatch_size << std::endl
            << "Number of epochs: " << epochs << std::endl
            << "Num train samples: " << num_train << std::endl
            << "Backend type: " << backend_type << std::endl
            << "Fashion-MNIST:" << fashion << std::endl;
  try {
    train_lenet(data_path, learning_rate, epochs, minibatch_size, backend_type,num_train,resume_path,fashion);
  } catch (tiny_dnn::nn_error &err) {
    std::cerr << "Exception: " << err.what() << std::endl;
  }
  return 0;
}
