#!/bin/sh
# Script de soumission Loadleveler pour un job sequentiel

# @ account_no = evolepidemics
# @ class = intel

# @ job_name = drugres_job_1
# @ output = $(job_name).out
# @ error  = $(job_name).err

# @ job_type = serial

# @ notify_user=gauthier.boaglio@cefe.cnrs.fr

# @ wall_clock_limit = 0:01:00,00:00:58
# @ queue


./drugres.sh

