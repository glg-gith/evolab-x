#!/bin/bash

# Run simulation
LAUNCHER_PATH=../DEPENDENCIES/py-launch.sh
SCRIPT_DIR=/home/golgauth/Documents/02_DEV/EVOLAB/ECLIPSE-WS_ELP/evolab-cython/src/Spatial
SCRIPT_NAME=BdmLsim2.py
SCRIPT_PARAMS="-i model.xml"
sh $LAUNCHER_PATH $SCRIPT_DIR $SCRIPT_NAME $SCRIPT_PARAMS


