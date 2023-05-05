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

	void train(NNTrainParams& params) {
		NNTrainSession s(&network,&params);
		s.start();
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