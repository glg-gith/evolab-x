##############################################################
		#	BUILD EVOLAB "cyelp" Python's Module	#
##############################################################

# Make Cython usable from DEPENDENCIES (for plateforms that do not have it installed)
PYTHONPATH=$PYTHONPATH:../../../DEPENDENCIES/Cython-0.18 export PYTHONPATH;
PATH=$PATH:../../../DEPENDENCIES/Cython-0.18/bin; export PATH;

# Make the "cyelp" Python Module ("cyelp.so")
CC="gcc"   \
CXX="g++"   \
#CFLAGS="-I../inc -I../../../DEPENDENCIES/python2.7/inc -I../../../DEPENDENCIES/gsl-1.15"   \
CFLAGS="-I../inc -I../../../DEPENDENCIES/gsl-1.15"   \
LDFLAGS="-L../lib"   \
    python setup.py build_ext --inplace
