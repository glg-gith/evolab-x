#!/bin/sh

# Script de soumission Loadleveler pour un job sequentiel
#@ job_name = BdmLsim4 (python)
#@ output = $(job_name).out
#@ error  = $(job_name).err
#@ job_type = serial
#@ wall_clock_limit = 0:30:00,00:29:59
#@ environment = COPY_ALL
#@ notify_user = gauthier.boaglio@cefe.cnrs.fr
#@ queue

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../../lib:../../../../DEPENDENCIES
export PYTHONPATH=$PYTHONPATH:../../setup

# Run simulation
python BdmLsim4.py -i model.xml
