##############################################################
		#	Run BdmLsim2 Demo - Python Version		#
##############################################################

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../../lib
export PYTHONPATH=$PYTHONPATH:../../setup

# Run simulation
valgrind --log-file=valgrind.report --show-reachable=no --leak-check=full --tool=memcheck --suppressions=valgrind-python.supp python ./BdmLsim4.py -i model.xml

#python BdmLsim4.py -i model.xml

