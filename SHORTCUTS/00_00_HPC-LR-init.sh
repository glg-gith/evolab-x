# Switch to gcc/g++ 4.4.6 instead of 4.6.2 (which is buggy on HPC@LR)
source /opt/cluster/gcc-4.4.3/refresh.sh
# Activate MPI features
export PATH=/usr/mpi/gcc/openmpi-1.4.3-qlc/bin:$PATH
export LD_LIBRARY_PATH=/usr/mpi/gcc/openmpi-1.4.3-qlc/lib64:$LD_LIBRARY_PATH


