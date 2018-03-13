echo. & echo '/******************** POST BUILD COPY TASKS ********************/' & echo.

REM  Copy .so & .h to evolab main project - Debug
REM cd ../Debug
REM cp -v libelp.so ../../evolab-project/Debug/Plugins/lib/; 
REM rm ../../evolab-project/Debug/Plugins/inc/*.*; 
REM cd ../src; 
REM find . -name '*.h' -exec cp -v {} ../../evolab-project/Debug/Plugins/inc/ \; ;

REM  Copy .so & .h to evolab main project - Release
REM cd ../Release
REM cp -v libelp.so ../../evolab-project/Release/Plugins/lib/; 
REM rm ../../evolab-project/Release/Plugins/inc/*.*; 
REM cd ../src; 
REM find . -name '*.h' -exec cp -v {} ../../evolab-project/Release/Plugins/inc/ \; ;

REM  Copy .so & .h to plugin devel project
COPY "./libelp.dll" "../../evolab-devel/lib/"
DEL "../../evolab-devel/inc/"*.*
FOR /R ../src %%f IN (*.h) DO COPY "%%f" "../../evolab-devel/inc/"

REM  Copy .so & .h to cython plugin devel project
COPY "./libelp.dll" "../../evolab-cython/lib/"
DEL "../../evolab-cython/inc/"*.*
FOR /R ../src %%f IN (*.h) DO COPY "%%f" "../../evolab-cython/inc/"

