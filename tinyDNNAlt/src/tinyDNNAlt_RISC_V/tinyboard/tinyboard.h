#pragma once
namespace tinyboard {
	const std::vector<std::string> topics{
		"/tmp/tinyboard.topics.epochs",
		"/tmp/tinyboard.topics.accuracy"
	};
}
#include "tinyboard/pipehandler.h"
#include "tinyboard/broker.h"
