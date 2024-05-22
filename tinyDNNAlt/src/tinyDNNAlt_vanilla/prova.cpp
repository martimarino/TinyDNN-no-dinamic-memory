#include "tiny_dnn/tiny_dnn.h"
#include "etl/platform.h"
#include "tiny_dnn/activations/sigmoid_layer.h"

#include <future>
#include <iostream>
#include <thread>

using namespace tiny_dnn;
using namespace tiny_dnn::activation;
using namespace tiny_dnn::layers;

void construct_mlp() {
    network<sequential> net;

    net << fc(10 * 10, 5) << tiny_dnn::activation::tanh();
}

int main() {
	construct_mlp();
	return 0;
}

