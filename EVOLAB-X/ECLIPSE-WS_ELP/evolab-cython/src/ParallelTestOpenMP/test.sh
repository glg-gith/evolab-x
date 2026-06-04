##############################################################
		#	Run Parallel Demo - Using "nogil"		#
##############################################################

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../../lib:../../../../DEPENDENCIES
export PYTHONPATH=$PYTHONPATH:../../setup

# Run simulation
python test.py
