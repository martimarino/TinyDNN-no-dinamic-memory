/*
    Copyright (c) 2019, Federico Rossi and the respective contributors
    All rights reserved.

    Use of this source code is governed by a BSD-style license that can be found
    in the LICENSE file.
*/
#pragma once 

#include "tiny_dnn/tiny_dnn.h"
#include <unistd.h>
#include <cstdlib>
#include <signal.h>
#include <functional>
using namespace tiny_dnn;
#ifdef QT_BOARD
using namespace tinyboard;
#endif


std::function<void(int)> callback_wrapper;
void callback_function(int value) {
  callback_wrapper(value);
}


class NNTrainSession
{
private:
	NNTrainParams* _params;
	tiny_dnn::progress_display* _display;
  	tiny_dnn::timer _timer;
  	tiny_dnn::float_t _elapsed=0;
	network<tiny_dnn::sequential>* _model;
	uint32_t _eepochs=0;
	#ifdef QT_BOARD
		tinyboard::TopicHandler* _accuracy_publisher;
	#endif

	bool _interrupted = false;
	

	void _catchInt(int sigNum) {
		if(_interrupted == true) {
			std::cout << "Forcing exit" << std::endl;
			exit(sigNum);
		}
		std::cout << "Interrupted, training will be stopped at the end of the epoch\n and the model will be saved\n"
				  << "Press CTRL+C again to force close the training" << std::endl;
		_interrupted = true;
	}

public:
	NNTrainSession(network<tiny_dnn::sequential>* model,NNTrainParams* params) {
		this->_params = params;
		this->_display = new tiny_dnn::progress_display(params->timages.size());
		this->_model = model;
		#ifdef QT_BOARD
		_accuracy_publisher = new tinyboard::TopicHandler(tinyboard::topics[0]);
		_accuracy_publisher->make();
		#endif
		callback_wrapper = std::bind(&NNTrainSession::_catchInt,this,std::placeholders::_1);
		struct sigaction sigIntHandler;
	    sigIntHandler.sa_handler = callback_function;
	    sigemptyset(&sigIntHandler.sa_mask);
	    sigIntHandler.sa_flags = 0;
	    sigaction(SIGINT, &sigIntHandler, NULL);
	}


	void _on_enumerate_minibatch() {
		(*_display)+=_params->minibatch_size;
	}

	void _on_enumerate_epoch() {
		++_eepochs;
		tiny_dnn::float_t epochTime = _timer.elapsed();
		_elapsed+=epochTime;
		tiny_dnn::result res = _model->test(_params->vimages, _params->vlabels);
		float accuracy = (float(res.num_success)/float(_params->vimages.size()));	
		#ifdef QT_BOARD
		_accuracy_publisher->writeObj(accuracy);
		#endif	
		std::cout << "\nEpoch " << _eepochs << " completed in " << epochTime << " s\nSession time: " << _elapsed << " s\nValidation accuracy: " << accuracy << std::endl;
		if(_interrupted) {
			_model->save("dump_epoch_"+std::to_string(_eepochs-1)+".tinymodel");
			_interrupted = false;
		}	
		_display->restart(_params->timages.size());
		_timer.restart();
	}


	template <class LossFunction>
	void start() {
		_model->train<LossFunction>(*(_params->optimizer), _params->timages, _params->tlabels, _params->minibatch_size,
                          _params->epochs, 
                          std::bind(&NNTrainSession::_on_enumerate_minibatch,this),
                          std::bind(&NNTrainSession::_on_enumerate_epoch,this),
                          false);
	}


	
};