#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include "tiny_dnn/tiny_dnn.h"
#include "omp.h"

/*
  REPORT_LEVEL
  0 -> Only final test+train sets evaluation
  1 -> Final evaluation + test set evaluation on each epoch
  2 -> Final evaluation + test and train set evaluation on each epoch
*/
int REPORT_LEVEL=1; 

void worker_epoch_callback(tiny_dnn::timer& train_timer,
                                  tiny_dnn::timer& comm_timer,int& epoch,
                                  int& epochs_interval,
                                  tiny_dnn::network<tiny_dnn::sequential>& nn,
                                  tiny_dnn::float_t& total_elapsed,
                                  tiny_dnn::float_t& communication_elapsed,
                                  int rank,
                                  int n_train_epochs,
                                  std::vector<tiny_dnn::vec_t>& test_images,
                                  std::vector<tiny_dnn::label_t>& test_labels,
                                  tiny_dnn::float_t& final_accuracy
                                  ) {
    total_elapsed+=train_timer.elapsed();
		std::cout << std::endl << "[NODE " << rank <<  "] Epoch " << epoch << "/" << n_train_epochs << " finished. "
							<< train_timer.elapsed() << "s elapsed." << std::endl;
		comm_timer.restart();
	
  //Start of comunication part
	//every tot epochs do this
	if(epoch%epochs_interval==0 || epoch==n_train_epochs){ 	
		std::vector<float_t> output_buffer;
		for ( int level=0; level< nn.depth(); level ++){
			std::vector<vec_t*> weights = nn[level]->weights();  
			if (weights.empty() || (*weights[0]).empty()) {
				continue;
			}
			for (size_t i = 0; i < weights.size(); i++) {
				vec_t &w   = *weights[i];	
				for (size_t j = 0; j < w.size(); j++) {
					output_buffer.push_back(w[j]);		
				}	
			}
		}	

		MPI_Send(output_buffer.data(),output_buffer.size(),MPI_FLOAT,0,201,MPI_COMM_WORLD);
		
		//wait for the new network
		MPI_Recv(output_buffer.data(),output_buffer.size(),MPI_FLOAT,0,201,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

		
		//reinitialize the weights vector
		int index=0;
		for ( int level=0; level< nn.depth(); level ++){
			std::vector<vec_t*> weights = nn[level]->weights();  	
			if (weights.empty() || (*weights[0]).empty()) {
				continue;
			}
			for (size_t i = 0; i < weights.size(); i++) {
				vec_t &w   = *weights[i];	
				for (size_t j = 0; j < w.size(); j++) {
					w[j]=output_buffer.at(index) ; 	
					index++;
				}	
			}
		}	
	}
	communication_elapsed+=comm_timer.elapsed();
	std::cout<< "[NODE " << rank  << "] Communication elapsed time: " << communication_elapsed<< std::endl;
	++epoch;
	if(REPORT_LEVEL > 0) {
	  tiny_dnn::result res = nn.test(test_images, test_labels);
		std::cout << "[NODE " << rank  << "] Test: " << res.num_success << "/" << res.num_total << std::endl;
		if (epoch-1==n_train_epochs){
			final_accuracy=(float)res.num_success/res.num_total;
			std::cout<< "[NODE " << rank  <<"] Final accuracy:"<<final_accuracy<<std::endl;
    }
  }
  train_timer.restart();
}

static void master_handler(int epochs, int epochs_interval, int size, double learning_rate, int num_train, int minibatch_size){	
	tiny_dnn::timer t1;
	int cicles = epochs/epochs_interval + int(epochs%epochs_interval!=0);  //if module!=0 do an extra final cicle
	int tot_cicles = cicles;
	int buf_size=-1;	
	std::vector<float*> recv_buffers(size-1);
	float* output_buffer;
	while(cicles>0){  
		//receive buffer from working nodes
    //#pragma omp parallel for
		for(int i = 1; i < size; ++i) {		
      std::cout << "Waiting parallel for " << i << std::endl;	 
			
      if (cicles == tot_cicles){ //if first cicle we read the dimension and allocate memory for big buffer
				if (buf_size == -1){
            MPI_Status status; 
            MPI_Probe(MPI_ANY_SOURCE,201,MPI_COMM_WORLD,&status);
            MPI_Get_count(&status, MPI_FLOAT, &buf_size);
				}
				recv_buffers[i-1] = new float[buf_size];					
			}	 		   				
			MPI_Recv(recv_buffers[i-1],buf_size,MPI_FLOAT,i,201,MPI_COMM_WORLD,MPI_STATUS_IGNORE); //bloccante
		}
		//compute mean and fill output_buffer
		if (cicles == tot_cicles){
			output_buffer= new float[buf_size];
		}
		float sum;
		for (size_t j = 0; j < buf_size; j++) {
				sum = 0.0;
				for (size_t i = 0; i < recv_buffers.size(); i++){
					sum += recv_buffers[i][j];
				}
				output_buffer[j]=sum/recv_buffers.size();
		}	
 
    //#pragma omp parallel for
		for(int i = 1; i < size; ++i) {	//send back to every node
			MPI_Send(output_buffer,buf_size,MPI_FLOAT,i,201,MPI_COMM_WORLD); //blocking
		}
		cicles--;
	}
	
	//receive final results from every node
	int performace_size=3;
	std::vector<float*> performance_buffers(size-1);
	for(int i = 1; i < size; ++i) { 
	
			performance_buffers[i-1] = new float[performace_size];	
			MPI_Recv(performance_buffers[i-1],performace_size,MPI_FLOAT,i,201,MPI_COMM_WORLD,MPI_STATUS_IGNORE);  			 
	}
	//compute mean of times
		float * result_buffer=new float[performace_size-1];
		float accuracy = performance_buffers[0][performace_size-1];
		float sum;
		for (size_t j = 0; j < performace_size-1; j++) {	//the last element  is the same for every buffer. dont compute the mean
				sum = 0.0;
				for (size_t i = 0; i < performance_buffers.size(); i++){
					sum += performance_buffers[i][j];
				}
				result_buffer[j]=sum/performance_buffers.size();
		}
		tiny_dnn::float_t total_time = t1.elapsed();
		
		std::string filename = make_filename(learning_rate,epochs,minibatch_size,num_train,"../../plot/MasterFinalPerformances",".txt");
		std::ofstream of(filename);
		of << "Average comunication time: " << result_buffer[1] << std::endl;
		of << "Average time : " << result_buffer[0] << std::endl;
		of<< "Total master time: "<< total_time<< std::endl;
		of<< "Final accuracy: "<< accuracy<< std::endl;
	return;
}

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
    using tanh = tiny_dnn::activation::elu;  //prova tanh
    using sigmoid = tiny_dnn::activation::sigmoid;
    using tiny_dnn::core::connection_table;
    using padding = tiny_dnn::padding;
    using smax = tiny_dnn::activation::softmax;
    using globalpool = tiny_dnn::global_average_pooling_layer;
	 //input w, input h, kernel dim, input channels, output depth (bias number, 1 bias per ogni kernel)
    nn << conv(32, 32, 3, 1, 32,   // C1, 1@32x32-in, 32@32x32-out	288[matrice pesi=3x3x32(32 filtri da 3x3)], 32[bias vector].    
               padding::same, true, 1, 1, 1, 1, backend_type)
       << tanh()
       << max_pool(32, 32, 32, 2)   // S2, 32@32x32-in, 32@16x16-out
       << conv(16, 16, 3, 32, 32,   // C2, 32@16x16-in, 32@16x16-out			//9216 = 3x3x32x32
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
       << fc(32,10,true)				//320,10
       << smax();
}

static void train_lenet(const std::string &data_dir_path,
                        double learning_rate,
                        const int n_train_epochs,
                        const int n_minibatch,
                        tiny_dnn::core::backend_t backend_type,const int num_train,std::string& resume_path,bool fashion = false,
						int rank=-1,
						int tot_working_nodes=1,
						int epochs_interval=5) {
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

  std::cout << "[NODE " << rank  << "] Loading datasets" << std::endl;

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


//Dataset split over the nodes 
	int tot_each_node=num_train/tot_working_nodes;	
	int tot_test = std::min((double)test_o_images.size(), tot_each_node*0.2);  

  std::cout << "[NODE " << rank  << "] Macrobatch size (train): " << tot_each_node << std::endl;
  std::cout << "[NODE " << rank  << "] Macrobatch size (test): " << tot_test << std::endl;

	std::vector<tiny_dnn::label_t> train_labels(train_o_labels.begin()+((rank-1)*tot_each_node),train_o_labels.begin()+(rank*tot_each_node)), 	
													test_labels(test_o_labels.begin(), test_o_labels.begin()+tot_test);			//each node tests on the same dataset portion
	std::vector<tiny_dnn::vec_t> train_images(train_o_images.begin()+((rank-1)*tot_each_node),train_o_images.begin()+(rank*tot_each_node)), 
													test_images(test_o_images.begin() , test_o_images.begin()+tot_test);

  std::cout << "[NODE " << rank << "] Training started" << std::endl;
	//tiny_dnn::progress_display disp(train_images.size());
	tiny_dnn::timer t;
	tiny_dnn::timer t2;

  optimizer.alpha *=
    std::min(tiny_dnn::float_t(4),
             static_cast<tiny_dnn::float_t>(sqrt(n_minibatch) * learning_rate));

  int epoch = 1;
  float final_accuracy=0.0;
  tiny_dnn::float_t total_elapsed = tiny_dnn::float_t{0.0};
  tiny_dnn::float_t comunication_elapsed = tiny_dnn::float_t{0.0};
  std::vector<unsigned int> test_succ;
  std::vector<unsigned int> train_succ;
  
  //create callback, called at the end of each epoch 
	auto on_enumerate_epoch = [&]() {
		worker_epoch_callback(t,t2,epoch,epochs_interval,nn,total_elapsed,comunication_elapsed,rank,n_train_epochs,test_images,test_labels,final_accuracy);
  };

	auto on_enumerate_minibatch = [&]() { /*disp += n_minibatch;*/};

	// training
	nn.train<tiny_dnn::cross_entropy_multiclass>(optimizer, train_images, train_labels, n_minibatch,
						  n_train_epochs, on_enumerate_minibatch,
						  on_enumerate_epoch,false);
						  
	//send my TIMES to master.					  
	float * performance_buffer = new float[3];
	performance_buffer[0]=total_elapsed;  //training time
	performance_buffer[1]=comunication_elapsed;  //tempo di comunicazione col master
	performance_buffer[2]=final_accuracy; 
	MPI_Send(performance_buffer,3,MPI_FLOAT,0,201,MPI_COMM_WORLD);		
	
	//only the first node saves the model
	if(rank==1){ //final report and model, made once.
		/*if(REPORT_LEVEL>0) {
			make_train_report(test_succ,learning_rate,n_train_epochs,n_minibatch,num_train,"../../plot/epochsPerf"); 
		if(REPORT_LEVEL>1)
			make_train_report(train_succ,test_succ,learning_rate,n_train_epochs,n_minibatch,num_train,"../../plot/epochsPerf");
		}*/
		make_final(nn,learning_rate,n_train_epochs,n_minibatch,num_train,test_labels,train_labels,test_images,train_images,total_elapsed);
	}
	std::cout << "end training. Node: "<<rank<<". Total time: " << total_elapsed << std::endl;
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
//inizializzazione OPENMPI 
  int rank=0,size=0,len=0,message=104;
  char version[MPI_MAX_LIBRARY_VERSION_STRING];
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Get_library_version(version, &len);	

  unsigned int seed = 132145;
  tiny_dnn::set_random_seed(seed);
  double learning_rate                   = 1;
  int epochs                             = 30;
  std::string data_path                  = "";
  int minibatch_size                     = 16;
  int num_train                          = 5000;
  int epochs_interval				=5;
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
	}else if(argname == "--epochs_interval"){	//new argument to set the number of epochs after which the master receives updates
		epochs_interval= atoi(argv[count + 1]);
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

//MPI
	if(rank==0){	
		//MASTER
  std::cout << "Running with the following parameters:" << std::endl
            << "Data path: " << data_path << std::endl
            << "Learning rate: " << learning_rate << std::endl
            << "Minibatch size: " << minibatch_size << std::endl
            << "Number of epochs: " << epochs << std::endl
            << "Num train samples: " << num_train << std::endl
            << "Backend type: " << backend_type << std::endl
            << "Fashion-MNIST:" << fashion << std::endl
			<< "Report Level:" << REPORT_LEVEL<<std::endl;    
		master_handler(epochs,  epochs_interval,size, learning_rate,num_train ,minibatch_size);

	}else{	
		//SLAVE
		try { 
			train_lenet(data_path, learning_rate, epochs, minibatch_size, backend_type,num_train,resume_path,fashion,rank,size-1,epochs_interval);
		} catch (tiny_dnn::nn_error &err) {
			std::cerr << "Exception: " << err.what() << std::endl;
		}
	}	
	MPI_Finalize();
	return 0;
}
