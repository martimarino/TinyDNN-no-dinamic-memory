#include "tiny_dnn/tiny_dnn.h"
#include "etl/platform.h"
#include "tiny_dnn/activations/sigmoid_layer.h"

#include <iostream>
#include <vector>

using namespace tiny_dnn;
using namespace tiny_dnn::activation;
using namespace tiny_dnn::layers;

#define IMG_SIZE 5

tensor_t test_image() {
    
    const size_t rows = 1;
    const size_t cols = IMG_SIZE*IMG_SIZE;

    tensor_t image(rows, vec_t(cols));

    for (size_t i = 0; i < cols; ++i) {
        image[0][i] = static_cast<float>(rand()) / RAND_MAX;  // Valori casuali tra 0 e 1
    }
    
    return image;
}


tensor_t construct_mlp(const tensor_t &input) {
    network<sequential> net;

    // Costruisci il modello
    net << fc(IMG_SIZE * IMG_SIZE, 3) << tiny_dnn::activation::tanh();

    std::cout << "Input[0][0]: " << input[0][0] << std::endl;
    std::cout << "Input size: " << input[0].size() << std::endl;

    // Controlla le dimensioni dell'input
    if (input.size() != IMG_SIZE*IMG_SIZE) {
        throw std::runtime_error("Dimensione dell'input non corretta!");
    }
    if (input[0].size() != 1) {
        throw std::runtime_error("Dimensione del sotto vettore non corretta!");
    }

    // Propaga l'immagine di input attraverso la rete
    etl::vector<tensor_t, 1> v;
    v[0] = input;
    return net.predict(input);
}

int main() {
    try {
        // Genera un'immagine di prova
        tensor_t image = test_image();

        // Stampa l'immagine di prova
        std::cout << "Test image:" << std::endl;
        for (int i = 0; i < IMG_SIZE*IMG_SIZE; ++i) {
            std::cout << image[0][i] << " ";
        }

        // Ottieni l'output della rete neurale
        tensor_t output = construct_mlp(image);

        // Stampa l'output della rete neurale
        std::cout << "Network output:" << std::endl;
        for (const auto& val : output[0]) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Errore: " << e.what() << std::endl;
    }

    return 0;
}
