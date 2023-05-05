#include <iostream>
#include <sstream>
#include <fstream>
//#define CNN_USE_POSIT
//#define CNN_CUSTOM_DOT_PRODUCT
#include "tiny_dnn/tiny_dnn.h"

static std::string make_filename(double learning_rate,int n_train_epochs,int n_minibatch,int num_train,std::string prefix,std::string suffix,bool fashion=0) {
  std::stringstream ss;
  ss << prefix;
  #ifdef CNN_USE_POSIT 
    ss << "_posit_" << tiny_dnn::float_t::vtotalbits << "_" << tiny_dnn::float_t::vesbits << "_";
  #elif defined CNN_USE_DOUBLE
    ss << "_double_";
  #else
    ss << "_float_";
  #endif
  ss << "lr_" << learning_rate
     << "_ep_" << n_train_epochs
     << "_mb_" << n_minibatch
     << "_nt_" << num_train;
  if(fashion) ss << "_fashion_mnist_";
  ss << suffix;
  std::string filename = ss.str();
  return filename;  
}

static void make_train_report(std::vector<unsigned int> train,std::vector<unsigned int> test,double learning_rate,int n_train_epochs,int n_minibatch,int num_train,std::string prefix) {
  std::string filename = make_filename(learning_rate,n_train_epochs,n_minibatch,num_train,prefix,".csv");
  std::ofstream file_out(filename);
  file_out << "EPOCH,TRAIN_SUCC,TEST_SUCC" << std::endl;
  int e = 0;
  for(auto i=train.begin(),j=test.begin();i!=train.end() && j!=test.end();i++,j++,e++)
    file_out << e << "," << *i << "," << *j << std::endl;
}

static void make_train_report(std::vector<unsigned int> test,double learning_rate,int n_train_epochs,int n_minibatch,int num_train,std::string prefix) {
  std::string filename = make_filename(learning_rate,n_train_epochs,n_minibatch,num_train,prefix,".csv");
  std::ofstream file_out(filename);
  file_out << "EPOCH,TEST_SUCC" << std::endl;
  int e = 0;
  for(auto i=test.begin();i!=test.end();i++,e++)
    file_out << e << "," << *i << std::endl;
}

static void make_final(tiny_dnn::network<tiny_dnn::sequential> &nn,
                       double learning_rate,
                       int n_train_epochs,
                       int n_minibatch,
                       int num_train,
                       std::vector<tiny_dnn::label_t> test_labels,
                       std::vector<tiny_dnn::label_t> train_labels,
                       std::vector<tiny_dnn::vec_t> test_images,
                       std::vector<tiny_dnn::vec_t> train_images,
                       double total_elapsed) {
  std::string filename = make_filename(learning_rate,n_train_epochs,n_minibatch,num_train,"../../plot/finalPerf",".txt");
  std::ofstream of(filename);
  //of << "Total time: " << total_elapsed << std::endl;
  of << "============TEST PHASE==============" << std::endl;
  nn.test(test_images, test_labels).print_detail(of);
  of << "============TRAIN PHASE==============" << std::endl;
  nn.test(train_images, train_labels).print_detail(of);

  //SAVE THE NETWORK MODEL+WEIGHTS
  std::string net_name = make_filename(learning_rate,n_train_epochs,n_minibatch,num_train,"../../pretrained_models/net",".model");
  nn.save(net_name);  
}


void make_test_report(std::vector<tiny_dnn::vec_t>& imgs,int successes,std::vector<unsigned int> times,unsigned int total,double meant,std::string prefix) {
  std::stringstream ss;
  ss << prefix;
  #ifdef CNN_USE_POSIT 
    ss << "posit_" << tiny_dnn::float_t::vtotalbits << "_" << tiny_dnn::float_t::vesbits;
    #ifdef CNN_TAB_TYPE
      ss << "_tab";
    #endif
  #elif defined CNN_USE_DOUBLE
    ss << "double";
  #else
    ss << "float";
  #endif  
  std::string filename = ss.str();
  std::ofstream of(filename+".txt");
  std::ofstream oft(filename+"_times.txt");
  of << "Accuracy: " << (double)successes/(double)imgs.size() << std::endl;
  of << "Time elapsed: " << total << std::endl;
  of << "Mean t: " << meant << std::endl;
  for(auto i: times) oft << i << std::endl;
}


void startLog(bool all) {
  #ifdef CNN_USE_POSIT
    #ifdef CNN_TAB_TYPE
      if(all)
        CNN_TAB_TYPE::_logger.setLogLevel(LogLevel::ALL);
      else
        CNN_TAB_TYPE::_logger.setLogLevel(LogLevel::STAT);
    #endif
  #endif
}

void endLog(std::string logFile) {
  #ifdef CNN_USE_POSIT
    #ifdef CNN_TAB_TYPE
      CNN_TAB_TYPE::_logger.save(logFile);
      CNN_TAB_TYPE::_logger.setLogLevel(LogLevel::NONE);      
    #endif
  #endif  
}
