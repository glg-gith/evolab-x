##############################################################
		#	Run BdmLsim2 Demo - Python Version		#
##############################################################

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../../lib:../../../../DEPENDENCIES
export PYTHONPATH=$PYTHONPATH:../../setup

# Run simulation
python DrugRes.py -i model.xml
