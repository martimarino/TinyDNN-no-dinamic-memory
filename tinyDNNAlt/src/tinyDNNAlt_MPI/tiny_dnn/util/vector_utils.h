#pragma once 

#include "tiny_dnn/tiny_dnn.h"
using namespace tiny_dnn;

namespace tiny_dnn {
	namespace vutil {
		std::vector<unsigned int> genRandomIndices(size_t maxIndex,size_t numIndices) {
			if(maxIndex < numIndices)
				    throw tiny_dnn::nn_error("Max index must be greater or equal than number of indices");
			std::unordered_set<unsigned int> indices;
			std::vector<unsigned int> distincts;
			while(distincts.size() < numIndices) {
				unsigned int index = tiny_dnn::uniform_rand<unsigned int>(0,maxIndex);
				auto res = indices.insert(index);
				if(std::get<1>(res))
					distincts.push_back(index);
			}
			return distincts;
		}

		template <class T>
		std::vector<T> randomSample(const std::vector<T>& in,const std::vector<unsigned int>& indices) {
			std::vector<T> out;
			for(auto i: indices)
				out.push_back(in[i]);
			return out;
		}	


		template <class I,class L>
		void randomShuffle(std::vector<I>& img,std::vector<L>& lab) {
			std::vector<unsigned int> v(img.size(),0);
			for(unsigned int i = 0; i < v.size(); ++i)
				v[i] = i;
			std::shuffle(v.begin(),v.end(),random_generator::get_instance()());
			img = randomSample(img,v);
			lab = randomSample(lab,v);
		}


	}

}