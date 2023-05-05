/*
    Copyright (c) 2019, Federico Rossi and the respective contributors
    All rights reserved.

    Use of this source code is governed by a BSD-style license that can be found
    in the LICENSE file.
*/
#pragma once 

#include "tiny_dnn/tiny_dnn.h"


class NNModel
{
private:


protected:
	std::string modelName;
	tiny_dnn::network<tiny_dnn::sequential> network;
public:
	NNModel(std::string modelName,
			tiny_dnn::network<tiny_dnn::sequential>& network)
	{
		this->modelName = modelName;
		this->network = network;
	};

	template <class LossFunction>
	void train(NNTrainParams& params) {
		NNTrainSession s(&network,&params);
		s.start<LossFunction>();
	}

	void save(std::string path) {
		network.save(path);
	}

	void save() {
		network.save(modelName+".tinymodel");
	}

	void dumpWeights() {
		network.save(modelName+".tinymodel",content_type::weights);
	}
	

	tiny_dnn::vec_t infer(tiny_dnn::vec_t in) {
		return this->network.predict(in);
	}

};