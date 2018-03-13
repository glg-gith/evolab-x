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
                if [ -d "./elps/html" ]; then
                    rm -r "./elps/html"
                    rm -r "./elps/latex"
                    #mkdir -p "./elps/html"
                fi
				cd ./elps
				doxygen Doxyfile
				cd ..
            ;;
        --cyelp)
                if [ -d "./cyelp/html" ]; then
                    rm -r "./cyelp/html"
                    #mkdir -p "./cyelp/html"
                fi
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


