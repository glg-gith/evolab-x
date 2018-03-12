#!/bin/bash

##############################################################
		#	BUILD EVOLAB C++ & Python Documentations	#
##############################################################

if [ $# -eq 0 ]
then
	echo "No arguments ? Type '$0 --help' !"
	exit 0
fi

while test $# -gt 0
do
    case "$1" in
        --help)
				echo "USAGE : $0 [--help]"
				echo "           [--cyelp Build Cython documentation]"
				echo "           [--elps  Build C++ shared library documentation]"
				exit 0
            ;;
        --elps)
				cd ./elps
				doxygen Doxyfile
				cd ..
            ;;
        --cyelp)
				cd ./cyelp
				sh ./build_doc.sh
				cd ..
            ;;
        --*) 
				echo "Bad option $1 ! (type $0 --help)"
				exit 0
            ;;
        *) 
				echo "Bad argument $1 ! (type $0 --help)"
				exit 0
            ;;
    esac
    shift
done


