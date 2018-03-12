#!/bin/bash

# add libelps.so to LD_LIBRARY_PATH
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:../../ECLIPSE-WS_ELP/evolab-cython/lib"
#export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/home/golgauth/Documents/02_DEV/EVOLAB/ECLIPSE-WS_ELP/evolab-cython/lib"

# add cyelp.so to PYTHONPATH
export PYTHONPATH="$PYTHONPATH:../../ECLIPSE-WS_ELP/evolab-cython/setup"
#export PYTHONPATH="$PYTHONPATH:/home/golgauth/Documents/02_DEV/EVOLAB/ECLIPSE-WS_ELP/evolab-cython/setup"

# generate documentation
epydoc -v --name "EVOLAB-X1 - Python API Documentation - 0.0.5" -o ./html \
--css epydoc.css --url http://www.evolab-project.org --inheritance listed \
--graph all --no-private --docformat epytext \
    --navlink "<a href=\"http://www.evolab-project.org\"><img src=\"../elp-cy-eee.png\" style=\"margin:10px;\" /></a>
        <script>
			var link = parent.document.createElement('link'); 
			link.id = 'dynamic-favicon'; 
			link.type = 'image/png'; 
			link.rel = 'shortcut icon'; 
			link.href = '../logo-base-fav.png'; 
			parent.document.head.appendChild(link); 
			parent.document.head = parent.document.head || parent.document.getElementsByTagName('head')[0];
		</script>
        " \
    cyelp
