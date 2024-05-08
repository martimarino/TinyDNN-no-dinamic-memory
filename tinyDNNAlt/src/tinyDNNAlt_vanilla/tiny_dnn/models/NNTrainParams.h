/*
    Copyright (c) 2019, Federico Rossi and the respective contributors
    All rights reserved.

    Use of this source code is governed by a BSD-style license that can be found
    in the LICENSE file.
*/
#pragma once

#include "tiny_dnn/tiny_dnn.h"
#include "lightconf/lightconf/lightconf.hpp"
#include "lightconf/lightconf/config_format.hpp"


class NNTrainParams
{

public:
	uint32_t epochs;
	uint32_t num_train;
	uint32_t minibatch_size;
	tiny_dnn::float_t learning_rate;
	tiny_dnn::optimizer* optimizer;
	bool stochastic;
	etl::vector<tiny_dnn::label_t, MAX_INPUT_SIZE> tlabels, vlabels;
  	etl::vector<tiny_dnn::vec_t, MAX_INPUT_SIZE> timages, vimages;
	NNTrainParams(uint32_t epochs,
				  uint32_t num_train,
				  uint32_t minibatch_size,
				  tiny_dnn::float_t learning_rate,
				  bool stochastic,
				  tiny_dnn::optimizer* optimizer,
				  const etl::vector<tiny_dnn::label_t, MAX_INPUT_SIZE>& tlabels,
				  const etl::vector<tiny_dnn::label_t, MAX_INPUT_SIZE>& vlabels,
  				  const etl::vector<tiny_dnn::vec_t, MAX_INPUT_SIZE>& timages,
  				  const etl::vector<tiny_dnn::vec_t, MAX_INPUT_SIZE>& vimages) {
		this->epochs = epochs;
		this->num_train = num_train;
		this->learning_rate = learning_rate;
		this->stochastic =stochastic;
		this->optimizer = optimizer;		
		this->tlabels = tlabels;
		this->vlabels = vlabels;
		this->timages = timages;
		this->vimages = vimages;		
	} 

	NNTrainParams(std::string confPath) {
		lightconf::group config_group;
		std::ifstream stream(confPath,std::ios::in);
		std::string srcContent = std::string(std::istreambuf_iterator<char>(stream),
                std::istreambuf_iterator<char>());
		config_group = lightconf::config_format::read(srcContent);
		this->epochs = config_group.get<int>("params.epochs");
		this->learning_rate = tiny_dnn::float_t(config_group.get<float>("params.learning_rate"));
		this->num_train = config_group.get<int>("params.num_train");
		this->stochastic = config_group.get<bool>("params.stochastic");
		this->minibatch_size = config_group.get<int>("params.minibatch_size");
	}


	NNTrainParams(std::string confPath,tiny_dnn::optimizer* optimizer,
				      const etl::vector<tiny_dnn::label_t, MAX_INPUT_SIZE>& tlabels,
				      const etl::vector<tiny_dnn::label_t, MAX_INPUT_SIZE>& vlabels,
  				      const etl::vector<tiny_dnn::vec_t, MAX_INPUT_SIZE>& timages,
  				      const etl::vector<tiny_dnn::vec_t, MAX_INPUT_SIZE>& vimages): NNTrainParams(confPath) {
		this->optimizer = optimizer;		
		this->tlabels = tlabels;
		this->vlabels = vlabels;
		this->timages = timages;
		this->vimages = vimages;
	}


	void setTrainData(tiny_dnn::optimizer* optimizer,
				      const etl::vector<tiny_dnn::label_t, MAX_INPUT_SIZE>& tlabels,
				      const etl::vector<tiny_dnn::label_t, MAX_INPUT_SIZE>& vlabels,
  				      const etl::vector<tiny_dnn::vec_t, MAX_INPUT_SIZE>& timages,
  				      const etl::vector<tiny_dnn::vec_t, MAX_INPUT_SIZE>& vimages) {
		this->optimizer = optimizer;		
		this->tlabels = tlabels;
		this->vlabels = vlabels;
		this->timages = timages;
		this->vimages = vimages;
	}


};
	std::ostream& operator<<(std::ostream& stream, 
                     const NNTrainParams& params) {
		stream << "==========================================" << std::endl;
		stream << "============TINY DNN TRAIN PARAMS=========" << std::endl;
		stream << "==========================================" << std::endl;
		stream << "Epochs: " << params.epochs << std::endl;
		stream << "Sub-train samples: " << params.num_train << std::endl;
		stream << "Learning rate: " << params.learning_rate << std::endl;
		stream << "Stochastic: " << params.stochastic << std::endl;
		stream << "Minibatch size: " << params.minibatch_size << std::endl;
		stream << "==========================================" << std::endl;
		stream << "==========================================" << std::endl;
    	return stream;
 	}
