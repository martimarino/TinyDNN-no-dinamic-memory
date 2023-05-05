#include <iostream>
#include <vector>
#include <chrono>
using  ns = std::chrono::nanoseconds;
using  ms = std::chrono::milliseconds;
using get_time = std::chrono::steady_clock ;

void naive_madd(std::vector<float> x,std::vector<float> y) {
  float acc = 0;
  for(int i = 0; i < x.size(); ++i) {
	acc+=x[i]*y[i];
  }

}

double bench(std::vector<float> a,std::vector<float> b,int run_num) {
	long elapsed = 0;
	auto start = get_time::now();
	for(int i = 0; i < run_num; ++i) {
		naive_madd(a,b);
	}
    	auto end = get_time::now();
	elapsed+= std::chrono::duration_cast<ms>(end - start).count();	
	return double(elapsed);
}


int main(int argc, char **argv) {
	int vec_size = atoi(argv[1]);
	int run_num  = atoi(argv[2]);
	int reps = atoi(argv[3]);
	std::vector<float> a,b;

	srand(0x86);
	for(int i = 0; i < vec_size; ++i) {
		a.push_back(rand());
		b.push_back(rand());
	}

	double total = 0;
	
	for(int i = 0; i < reps; ++i) {
		total+=bench(a,b,run_num);
		std::cout << "\r" << (i+1) << "\\" << reps << std::flush;
	}
	std::cout << "\rTime (ms): " << double(total)/double(reps) << std::endl;
}
