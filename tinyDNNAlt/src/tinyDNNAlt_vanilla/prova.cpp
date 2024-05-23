#include "tiny_dnn/tiny_dnn.h"
#include "etl/platform.h"
#include "tiny_dnn/activations/sigmoid_layer.h"

#include <iostream>
#include <vector>

using namespace tiny_dnn;
using namespace tiny_dnn::activation;
using namespace tiny_dnn::layers;
using smax = tiny_dnn::activation::softmax;

#define IMG_SIZE 5

tensor_t test_image() {
    
    const size_t rows = 3;
    const size_t cols = IMG_SIZE*IMG_SIZE;

    tensor_t image(rows, vec_t(cols));

    for (size_t j = 0; j < rows; ++j){
        for (size_t i = 0; i < cols; ++i) {
            image[j][i] = static_cast<float>(rand()) / RAND_MAX;  // Valori casuali tra 0 e 1
        }
    }

    std::cout << "Test image:" << std::endl;
        
    for (size_t j = 0; j < rows; ++j){
        for (int i = 0; i < cols; ++i) {
            std::cout << image[j][i] << " ";
        }
    }
    std::cout << std::endl;

    return image;
}


tensor_t construct_mlp(const tensor_t &input) {
    network<sequential> net;
    // model
    net << fc(IMG_SIZE * IMG_SIZE, 3) << tiny_dnn::activation::tanh() << smax();

    // std::cout << "Input[0][0]: " << input[0][0] << std::endl;
    std::cout << "Input size: " << input[0].size() << std::endl;

    // Check dimensioni dell'input
    if (input.size() != 3) {
        throw std::runtime_error("Dimensione dell'input non corretta!");
    }
    if (input[0].size() != IMG_SIZE*IMG_SIZE) {
        throw std::runtime_error("Dimensione del sotto vettore non corretta!");
    }
    
    return net.predict(input);
}

int main() {
    try {
        // Genera un'immagine di prova
        tensor_t image = test_image();
        // Ottieni l'output della rete neurale
        tensor_t output = construct_mlp(image);
        
        std::cout << "\n** Network output **" << std::endl;
        std::cout << "Output size: " << output.size() << std::endl;
        std::cout << "Prob. classes: ";
            for (const auto& val : output[0]) 
                std::cout << val << " ";
        std::cout << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Errore: " << e.what() << std::endl;
    }
    return 0;
}
