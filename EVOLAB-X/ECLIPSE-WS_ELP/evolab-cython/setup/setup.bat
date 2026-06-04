REM ##############################################################
REM 		#	BUILD EVOLAB "cyelp" Python's Module	#
REM ##############################################################

REM Make Cython usable from DEPENDENCIES (for plateforms that do not have it installed)

REM PYTHONPATH=$PYTHONPATH:../../../DEPENDENCIES/Cython-0.17.4; export PYTHONPATH;
SET PYTHONPATH=%PYTHONPATH%;../../../DEPENDENCIES/Cython-0.18
SET PATH=%PATH%;../../../DEPENDENCIES/Cython-0.18/bin

REM SET PATH=%PATH%;D:/02_DEV/EVOLAB/EVOLAB-2013-05-02/ECLIPSE-WS_ELP/evolab-cython/lib


REM Make the "cyelp" Python Module ("cyelp.so")

REM CC="gcc"   \
REM CXX="g++"   \
REM CFLAGS="-I../inc -I../../../DEPENDENCIES/python2.7/inc -I../../../DEPENDENCIES/gsl-1.8/include"   \
REM LDFLAGS="-L../lib"   \
    python setup.py build_ext --inplace
