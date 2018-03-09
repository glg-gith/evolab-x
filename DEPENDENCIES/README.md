# Make dependencies ready for use & run a python script

1. If dependencies are not yet compiled:

        * Unzip sources, then

        * Run "build_dep.sh" with the options you need (build_dep.sh --help to display them)
        (i.e. "cd ../DEPENDENCIES; ./build_dep.sh --cy --numpy --scipy #--gsl")

2. Launch your python scripts with :

        * Thoses directories added to PYTHONPATH :
            $ export MY_DEP=.../DEPENDENCIES
            $ export PYTHONPATH=$PYTHONPATH:$MY_DEP/cython:$MY_DEP/numpy:$MY_DEP/scipy

        * Add those files to LD_LIBRARY_PATH :
            $ export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.../cython/bin

	OR, 

        * Use the "py-launch.sh" script with proper script path and argument to be passed
