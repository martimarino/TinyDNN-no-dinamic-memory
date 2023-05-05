reset
echo "======================================================================"
echo "                         TINYDNN TESTBENCH                            "
echo "======================================================================"
mkdir -p build
cd build
echo "Running cmake..."
cmake -DTAB8=ON -DTAB10=ON ..
echo "Done"
echo "Compiling train executables..."
#make trains_type -j$(nproc)
echo "Done."
echo "Compiling test executables..."
make cifar_tests_type -j$(nproc)
echo "Done."

cd examples/cifar10
train_exe="./cifar_train"
test_exe="./cifar_test"
p80="_p8_0"
p80tab="_posittab8"
p100="_p10_0"
p10tab="_posittab10"
p122="_p12_2"
p120="_p12_0"
p142="_p14_2"
p162="_p16_2"
p140="_p14_0"
p160="_p16_0"
summ_suffix="_summary"
mkdir -p ../plot
cd examples
models=$(ls ../../../pretrained_models)

if [ "$1" == "train" ]; then
	train_args="--data_path ../../data --epochs 1 --num_train 1 --report_level 0"
	reset
	echo "Training network with type float32"
	$train_exe $train_args --fashion 0
	clear
	echo "Training network with type posit16,2"
	$train_exe$p162 $train_args --fashion 0
	clear
	echo "Training network with type posit14,2"
	$train_exe$p142 $train_args --fashion 0
	clear
	echo "Training network with type posit12,2"
	$train_exe$p122 $train_args --fashion 0
fi
clear

for model in $models
do
	model_path="../../../pretrained_models/"$model
	echo "Testing $model with type float32..."
 	$test_exe $model_path ../../../data/cifar > /dev/null
	echo ""
	echo "Testing $model with type posit16,2..."
	$test_exe$p160 $model_path ../../../data/cifar > /dev/null
	echo ""
	echo "Testing $model with type posit14,2"
	$test_exe$p140 $model_path ../../../data/cifar > /dev/null
	echo ""
        echo "Testing $model with type posit12,2"
        $test_exe$p122 $model_path ../../../data/cifar > /dev/null
        echo "" 
	echo "Testing $model with type posit12,0"
	$test_exe$p120 $model_path ../../../data/cifar > /dev/null
	echo ""	
	echo "Testing $model with type posit10,0"
	$test_exe$p100 $model_path ../../../data/cifar > /dev/null
	echo ""
	echo "Testing $model with type posit8,0"
	$test_exe$p80 $model_path ../../../data/cifar  > /dev/null
	echo ""
	echo "Testing $model with type posit10,0 tab"
	$test_exe$p10tab $model_path ../../../data/cifar > /dev/null
	echo ""
	echo "Testing $model with type posit8,0 tab"
	$test_exe$p80tab $model_path ../../../data/cifar  > /dev/null
	mkdir -p ../../../plot/$model
	cp ../../../plot/* ../../../plot/$model
	find ../../../plot/$model/*.txt ! -name '*times*' -printf "\n%p\n" -exec cat {} \; > ../../../plot/$model/summary.txt
	rm -r ../../../plot/*.txt
	clear
done

