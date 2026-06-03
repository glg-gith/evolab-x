#!/bin/sh

# Script de soumission Loadleveler pour un job sequentiel
#@ job_name = bdmLsim4
#@ output = $(job_name).out
#@ error  = $(job_name).err
#@ job_type = serial
#@ wall_clock_limit = 00:15:00,00:14:00
#@ environment = COPY_ALL
#@ notify_user = gauthier.boaglio@gmail.com
#@ queue

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../lib:../../../DEPENDENCIES
./evolab-devel -i ../model.xml
