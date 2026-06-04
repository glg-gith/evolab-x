echo '\n/******************** POST BUILD COPY TASKS ********************/\n'

# Copy .so & .h to evolab main project - Debug
cd ../Debug
cp -v libelp.so ../../evolab-project/Debug/Plugins/lib/; 
rm ../../evolab-project/Debug/Plugins/inc/*.*; 
cd ../src; 
find . -name '*.h' -exec cp -v {} ../../evolab-project/Debug/Plugins/inc/ \; ;

# Copy .so & .h to evolab main project - Release
cd ../Release
cp -v libelp.so ../../evolab-project/Release/Plugins/lib/; 
rm ../../evolab-project/Release/Plugins/inc/*.*; 
cd ../src; 
find . -name '*.h' -exec cp -v {} ../../evolab-project/Release/Plugins/inc/ \; ;

# Copy .so & .h to plugin devel project
cd ../Release
cp -v libelp.so ../../evolab-devel/lib/; 
rm ../../evolab-devel/inc/*.*; 
cd ../src; 
find . -name '*.h' -exec cp -v {} ../../evolab-devel/inc/ \; ;

# Copy .so & .h to cython plugin devel project
cd ../Release
cp -v libelp.so ../../evolab-cython/lib/; 
rm ../../evolab-cython/inc/*.*; 
cd ../src; 
find . -name '*.h' -exec cp -v {} ../../evolab-cython/inc/ \; ;


# Copy .so & .h to QT-GUI project
cd ../Release
cp -v libelp.so ../../../GUI/EVOLAB-GUI/DEPS/lib/; 
rm ../../../GUI/EVOLAB-GUI/DEPS/inc/*.*; 
cd ../src; 
find . -name '*.h' -exec cp -v {} ../../../GUI/EVOLAB-GUI/DEPS/inc/ \; ;


