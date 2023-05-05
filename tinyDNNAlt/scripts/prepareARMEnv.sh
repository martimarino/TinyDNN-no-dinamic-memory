module load Generic-AArch64/RHEL/7/arm-hpc-compiler/19.2 Generic-AArch64/RHEL/7/arm-instruction-emulator/19.1 Generic-AArch64/RHEL/7/arm-reports/19.0.3 Generic-AArch64/RHEL/7/suites/arm-compiler-for-hpc/19.2 cmake
export CC=$(which armclang)
export CXX=$(which armclang++)
