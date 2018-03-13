#!/bin/sh
# Script de soumission Loadleveler pour un job sequentiel

# @ account_no = EVOLEPIDEMICS
# @ class = intel

# @ job_name = toto
# @ output = $(job_name).out
# @ error  = $(job_name).err

# @ job_type = serial

# @ wall_clock_limit = 0:01:00,00:00:58
# @ queue

/home/boagliog/bin/R-3.0.2/bin/R --file=/home/boagliog/bin/R-3.0.2/bin/test.r
