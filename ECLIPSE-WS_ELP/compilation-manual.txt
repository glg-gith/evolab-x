# To compile all C stuffs :

	cd ECLIPSE-WS_ELP
	make release (ou make debug)

# To clean all C stuffs :

	cd ECLIPSE-WS_ELP
	make clean

# To compile the shared library without Python dependencies :

	* Open "ECLIPSE-WS_ELP/evolab-shared/src/SimLab/SimCore/LabConsts.h" and
		uncomment "//#define NOPYTHON"
	* Open "ECLIPSE-WS_ELP/evolab-shared/Release/objects.mk" and
		remove "-lpython2.7"
	* Clean and recompile

# To compile Cython extension :

	cd ECLIPSE-WS_ELP/evolab-cython/setup
	./setup.sh


