#define MAX_INPUT_SIZE 1024 // considering 32 * 32
#define MAX_OUTPUT_SIZE 10

#define MAX_LABEL_SIZE 10
#define MAX_LAYERS 20
#define MAX_NODES 2560 // 20 * 128
#define MAX_TENSOR_SIZE 1024
// considering 32 * 32 * 3
#define MAX_CHANNEL_SIZE 3
#define MAX_SAMPLES 1000
#define MAX_CONNECTIONS 150000 // Numero di connessioni tra l'input layer e il primo hidden layer 1024*128 + Numero di connessioni tra i hidden layers 128 * 128 Numero di connessioni tra l'ultimo hidden layer e l'output layer 128 * 10
#define MAX_VSIZE 1024
#define MAX_WEIGTHS_SIZE 1024
#define MAX_BATCH_SIZE 8
