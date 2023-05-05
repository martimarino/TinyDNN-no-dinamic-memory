#pragma once
#include "tinyboard/pipehandler.h"
#include <mutex>
#include <thread>
#include <condition_variable>

namespace tinyboard {
	class Broker {
		std::map<std::string,std::queue<float>*> topic_data_;
		std::map<std::string,std::unique_ptr<std::mutex>> topic_mutex_;
		std::map<std::string,std::unique_ptr<std::condition_variable>> topic_cond_;
		std::vector<std::thread*> thread_pool_;
		bool stopped_ = false;

		void topicListener(tinyboard::TopicHandler th,std::string index) {
			std::cout << "Listening: " << index << std::endl;
			while(!stopped_) {
				float f = th.readObj<float>();
				std::cout << "[" << index << "] Received: " << f  << std::endl;
				insert(index,f);
			}
		}

	public:
		Broker() {
			for(unsigned int i = 0; i < tinyboard::topics.size(); ++i) {
				std::cout << "Setup: " << tinyboard::topics[i] << std::endl;
				tinyboard::TopicHandler th(tinyboard::topics[i]);
				th.make();
				topic_data_.insert(std::pair<std::string,std::queue<float>*>(tinyboard::topics[i], new std::queue<float>()));
				topic_mutex_.emplace(tinyboard::topics[i], std::make_unique<std::mutex>());
				topic_cond_.emplace(tinyboard::topics[i], std::make_unique<std::condition_variable>());
				thread_pool_.push_back(new std::thread(&Broker::topicListener,this,th,tinyboard::topics[i]));
			}
		}

		float fetch(std::string index) {
        	std::unique_lock<std::mutex> lock(*(topic_mutex_.find(index)->second));
			std::queue<float>* q = topic_data_.find(index)->second;
        	topic_cond_.find(index)->second->wait(lock,[=] {
        		return !q->empty();
        	});
			float r = q->front();
			q->pop();
			return r;
		}

		void insert(std::string index,float data) {
        	std::unique_lock<std::mutex> lock(*(topic_mutex_.find(index)->second));			
			topic_data_.find(index)->second->push(data);
        	topic_cond_.find(index)->second->notify_all();
		}
	};
} 