#export PYTHONPATH="$PYTHONPATH:./setup"
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:../../ECLIPSE-WS_ELP/evolab-cython/lib"
#make clean
#sphinx-apidoc -o . . /home/golgauth/Documents/02_DEV/EVOLAB/evolab-cython
make html

