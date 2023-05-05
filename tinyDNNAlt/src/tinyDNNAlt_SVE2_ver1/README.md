# tinyDNNAlt

This repository contains the tiny-DNN library, adapted to work with alternative 
representations for real numbers, such as the posit representation.

# MNIST All-in-one command
```sh
./all.sh
```
The above command will compile both training and testing examples and will execute the most
relevant ones producing result files in the folders *plot* and *pretrained_models*. 

If you want to execute it for only one particular type see instructions below.

# Build MNIST example(s)

```sh
mkdir -p build
cd build
cmake .. -DTAB8=ON -DTAB10=ON
make tests_type trains_type -j$(nproc)
```

The latter will compile the training and testing code for the different types of Posit and Float.

# Training

```sh
cd build/examples
# choose one type for training (for example Posit16,2)
./example_mnist_train_p16_2 --data_path ../../data --num_train 60000 --epochs 30 --report_level 2
# Above command will produce a trained model in the folder 'pretrained_models' and some report files
# based on the chosen report_level in the folder 'plot'
```

# Testing

```sh
cd build/examples
# choose one type for training (for example Posit10,0 tabulated)
./example_mnist_test_alt_posittab10 ../../pretrained_models/model_name ../../data
# Above command will produce some report files inside the 'plot' folder
```