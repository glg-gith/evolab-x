#!/bin/bash

###
# Launch a python script from given script path and given parameters
###

if [ "$#" -lt 2 ]
then
	echo "USAGE : $0 <SCRIPT LOCATION DIR> <SCRIPT NAME> <SCRIPT PARAMS>"
	exit 0
else
	DIR=$1
	FILE=$2
fi

PARAMS=""
COUNTER=0
for var in "$@"
do
	if [ $COUNTER -gt 1 ]	# Skip the first 2 parameters
	then
		PARAMS="$PARAMS$var "
	fi
	COUNTER=$((COUNTER+1))
done

while test $# -gt 0
do
    case "$1" in
        --help)
				echo "USAGE : $0 <SCRIPT LOCATION DIR> <SCRIPT NAME> <SCRIPT PARAMS>"
				exit 0
            ;;
        --*) 
				echo "Bad option $1 ! (type $0 --help)"
				exit 0
            ;;
#        *) echo "Bad argument $1 ! (type $0 --help)"
#            ;;
    esac
    shift
done

echo "Setting environment variables..."

# Cython project's root directory
export ROOT_DIR=`pwd`/..		# Customize the path here (we currently get the parent of "DEPENDENCIES")
export PROJECT_DIR=$ROOT_DIR/ECLIPSE-WS_ELP/evolab-cython

# Load the libelp.so
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PROJECT_DIR/lib

# Load the cyelp.so (Module ready for Python access)
export PYTHONPATH=$PYTHONPATH:$PROJECT_DIR/setup

# Other Python dependencies
export PY_DEPS=$ROOT_DIR/DEPENDENCIES
export PYTHONPATH=$PYTHONPATH:$PY_DEPS/cython:$PY_DEPS/numpy:$PY_DEPS/scipy


echo "LD_LIBRARY_PATH="$LD_LIBRARY_PATH
echo "PYTHONPATH="$PYTHONPATH


# Run a simulation
echo "\nEntering directory : "$DIR; cd $DIR
echo "\nRunning command : $FILE $PARAMS"; python $FILE $PARAMS
echo "\nLeaving directory : "$DIR; cd $PY_DEPS




