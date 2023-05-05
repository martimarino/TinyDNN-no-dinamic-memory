#include <iostream>
#include <vector>
#ifdef __ARM_FEATURE_SVE
#include <arm_sve.h>
#endif /* __ARM_FEATURE_SVE */
# include <chrono>
using  ns = std::chrono::nanoseconds;
using  ms = std::chrono::milliseconds;
using get_time = std::chrono::steady_clock ;

void sv_madd(std::vector<float> x,std::vector<float> y) {
	int32_t n = x.size(), i =0;
	svbool_t all = svptrue_b32();
	svbool_t Pg;
	float32_t acc = 0;
	for(int i = 0;
	    svptest_first(all,Pg=svwhilelt_b32(i, n));
            i+=svcntw()) {
		svfloat32_t x_vec = svld1(Pg,&x[i]);
		svfloat32_t y_vec = svld1(Pg,&y[i]);
		svfloat32_t r_vec = svmul_f32_m(Pg,x_vec,y_vec);
		acc+= svadda_f32(Pg,0,r_vec);
	}

}

double bench(std::vector<float> a,std::vector<float> b,int run_num) {
	long elapsed = 0;
	auto start = get_time::now();
	for(int i = 0; i < run_num; ++i) {
		sv_madd(a,b);
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
