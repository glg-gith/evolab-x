##############################################################
		#	Run DrugRes Demo - Python Version		#
##############################################################

REM export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../../lib
SET PATH=%PATH%;../../lib;../../../../DEPENDENCIES/libxml2/installed/bin;../../../../DEPENDENCIES/gsl-1.8/bin
REM export PYTHONPATH=$PYTHONPATH:../../setup
SET PYTHONPATH=%PYTHONPATH%;../../setup;

# Run simulation
python DrugRes.py -i model.xml
