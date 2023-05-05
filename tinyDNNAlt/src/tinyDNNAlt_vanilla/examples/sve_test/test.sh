######### TEST CONFIGURATION ############
CPP=aarch64-linux-g++
vector_size=$1
run_number=$2
reps=$3
#########################################


armclang++ -O1 test_naive.cpp -o naive_O1
armclang++ -Ofast -march=armv8-a+sve test.cpp -o sve
echo "Naive madd -O1 [NO AUTOVECT]"
armie -msve-vector-bits=2048 naive_O1 $vector_size $run_number $reps
echo "SVE madd -Ofast 128-bits"
armie -msve-vector-bits=128 sve $vector_size $run_number $reps
echo "SVE madd -Ofast 256-bits"
armie -msve-vector-bits=256 sve $vector_size $run_number $reps
echo "SVE madd -Ofast 512-bits"
armie -msve-vector-bits=512 sve $vector_size $run_number $reps
echo "SVE madd -Ofast 1024-bits"
armie -msve-vector-bits=1024 sve $vector_size $run_number $reps
echo "SVE madd -Ofast 2048-bits"
armie -msve-vector-bits=2048 sve $vector_size $run_number $reps

