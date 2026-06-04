##############################################################
		#	Run BdmLsim4 Demo - C++ Version		#
##############################################################

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../lib:../../../DEPENDENCIES

# Run simulation
valgrind --log-file=valgrind.report --show-reachable=yes --leak-check=full --tool=memcheck ./evolab-devel -i ../model.xml

#python DrugRes.py -i model.xml
