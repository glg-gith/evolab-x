#!/bin/bash

##############################################################
			#	BUILD EVOLAB DEPENDENCIES	#
##############################################################

# Little preliminary cleanup
#if [ -f ./libgsl.so ]
#then 
#	rm ./libgsl.so
#fi
#if [ -f ./libgslcblas.so ]
#then 
#	rm ./libgslcblas.so
#fi
#if [ -f ./libxml2.so ]
#then 
#	rm ./libxml2.so
#fi
#if [ -d ./python2.7/inc/numpy ]
#then 
#	rm ./python2.7/inc/numpy
#fi
#
#if [ -d ./cython ]
#then 
#	rm ./cython
#fi
#if [ -d ./numpy ]
#then 
#	rm ./numpy
#fi
#if [ -d ./scipy ]
#then 
#	rm ./scipy
#fi

INSTALL_DIR=.. # TODO : use it !!!

# Base dir gsl / gslcblas
GSL_DIR=./gsl-2.4
# Base dir xml2
XML2_DIR=./libxml2-2.9.1
# Base dir Cython
CY_DIR=./Cython-0.27.3
# Base dir NumPy
NUMPY_DIR=./numpy-1.14.1
# Base dir SciPy
SCIPY_DIR=./scipy-1.0.0

# Install postfix
POSTFIX="-installed"

# ln
MAKE_LINKS=false

if [ $# -eq 0 ]
then
	echo "No arguments ? Type '$0 --help' !"
	exit 0
fi

while test $# -gt 0
do
    case "$1" in
        --help)
				echo "USAGE : $0 [--help]"
				echo "           [--gsl Build GSL] [--cy Build Cython]"
				echo "           [--numpy Build NumPy] [--scipy Build SciPy]"
				echo "           [--xml2 Build libxml2 or link if existing...]"
				exit 0
            ;;
        --gsl)
				cd $GSL_DIR
				./configure #CC=icc --shared
				make
				cd ..
				ln -s $GSL_DIR/.libs/libgsl.so ./libgsl.so
				ln -s $GSL_DIR/cblas/.libs/libgslcblas.so ./libgslcblas.so
            ;;
        --cy)
                export PYTHONPATH=$PYTHONPATH:../$CY_DIR$POSTFIX/lib/python2.7/site-packages # Added 2018-03-08
				cd $CY_DIR
				python setup.py install --prefix="../$CY_DIR$POSTFIX"
				cd ..
				ln -s $CY_DIR$POSTFIX ./cython
            ;;
        --cy-root)
				cd $CY_DIR
				python setup.py install
				cd ..
            ;;
        --numpy)
                export PYTHONPATH=$PYTHONPATH:../$NUMPY_DIR$POSTFIX/lib/python2.7/site-packages # Added 2018-03-08
				cd $NUMPY_DIR
				python setup.py install --prefix="../$NUMPY_DIR$POSTFIX"
				cd ..
				ln -s $NUMPY_DIR$POSTFIX ./numpy
            ;;
        --numpy-root)
				cd $NUMPY_DIR
				python setup.py install
				cd ..
            ;;
        --scipy)
                export PYTHONPATH=$PYTHONPATH:../$SCIPY_DIR$POSTFIX/lib/python2.7/site-packages # Added 2018-03-08
				cd $SCIPY_DIR
				python setup.py install --prefix="../$SCIPY_DIR$POSTFIX"
				cd ..
				ln -s $SCIPY_DIR$POSTFIX ./scipy
            ;;
        --xml2)
				# Assuming libxml2 is on every computers (for the moment)
				if [ -f /usr/lib/libxml2.so.2 ]
				then
					ln -s /usr/lib/libxml2.so.2 ./libxml2.so;
				elif [ -f /usr/lib/x86_64-linux-gnu/libxml2.so.2 ]
				then
					ln -s /usr/lib/x86_64-linux-gnu/libxml2.so.2 ./libxml2.so;
				elif [ -f /usr/lib64/libxml2.so.2 ]
				then
					ln -s /usr/lib64/libxml2.so.2 ./libxml2.so;
				else
					echo "Error : 'libxml2.so.2' not found !";
				fi
            ;;
        --*) 
				echo "Bad option $1 ! (type $0 --help)"
				exit 0
            ;;
        *) 
				echo "Bad argument $1 ! (type $0 --help)"
				exit 0
            ;;
    esac
    shift
done


# Setup dyn links
#if [ $MAKE_LINKS=Cython-0.17.4true ]
#then
#	# GSL
#
#	# Cython
#	ln -s /usr/lib/pymodules/python2.7/numpy/core/include/numpy ./python2.7/inc/numpy
#	ln -s $CY_DIR "$CY_DIR-installed"
#
#
#	# XML2
#	#cd $XML2_DIR
#	#./configure #CC=icc
#	#make
#	#ln -s $XML2_DIR/.libs/libxml2.so ../libxml2.so
#
#	#cd ..
#fi





