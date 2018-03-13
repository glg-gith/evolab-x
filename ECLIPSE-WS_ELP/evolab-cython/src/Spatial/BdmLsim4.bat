REM ##############################################################
REM 		#	Run BdmLsim4 Demo - Python Version		#
REM ##############################################################

REM export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../../lib
SET PATH=%PATH%;../../lib;../../../../DEPENDENCIES/libxml2/installed/bin;../../../../DEPENDENCIES/gsl-1.8/bin
REM export PYTHONPATH=$PYTHONPATH:../../setup
SET PYTHONPATH=%PYTHONPATH%;../../setup;

REM # Run simulation
python BdmLsim4.py -i model.xml
