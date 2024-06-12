#include <iostream>
#include <etl/vector.h>
#include "etl/platform.h"
#include <cstdlib>
#include <cmath>

#define DIM1 3
#define DIM2 2

void softmax(const etl::vector<float, 5> &x, etl::vector<float, 5> &y){

    std::cout << "\nVettore x:   ";
    for(int i = 0; i < 5; i++)
    {
    std::cout << x[i] << " ";
    }
    const float alpha = *std::max_element(x.begin(), x.end());
    float denominator(0);
    
    for (size_t j = 0; j < 5; j++) {
      y[j] = exp(x[j] - alpha);
      denominator += y[j];
    }
    for (size_t j = 0; j < 5; j++) {
      y[j] /= denominator;
    }
  }


float test_sum() {

    int somma = 0;
    etl::vector<int, 5> vettore;

    for (int i = 0; i < 5; i++)
        vettore[i] = i;

    for (int i = 0; i < 5; i++)
        somma += vettore[i];

    return somma;
}


float test_vectors_scalar_product() {

    etl::vector<float, 5> vettore1;
    etl::vector<float, 5> vettore2;
    etl::vector<float, 5> risultato;
    etl::vector<float, 5> sm;

    for (int i = 0; i < 5; i++){
        vettore1[i] = i+1;
        vettore2[i] = i+1;
    }

    std::cout << "Risultato:   ";
    for(int i = 0; i < 5; i++)
    {
        risultato[i] = vettore1[i]*vettore2[i];
        std::cout << risultato[i] << " ";
    }

    sm.fill(0.0);
    softmax(risultato, sm);

    std::cout << "\n" ;

    std::cout << "Vettore sm:  ";
    float somma = 0;
    for(int i = 0; i < 5; i++){
        std::cout << sm[i] << " ";
        std::cout << " (int " << (int)sm[i] << ") ";
        somma += sm[i];
    }
    std::cout << "\n";
    std::cout << "Somma softmax: " << somma << "\n";

    return vettore1[0];
}

int main() {
    test_sum();
    test_vectors_scalar_product();
    return 0;
}