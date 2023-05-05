module load cmake gnu
cc_comp=$(which gcc)
cxx_comp=$(which g++)
export CC=$cc_comp
export CXX=$cxx_comp

cd ..
git pull
git lfs fetch
git lfs checkout
