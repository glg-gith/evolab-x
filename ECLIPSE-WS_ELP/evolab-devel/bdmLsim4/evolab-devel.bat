SET PATH=%PATH%;../lib;../../../libxml2/installed/bin;../../../DEPENDENCIES/gsl-1.8\bin
SET PATH=%PATH%;..\lib;..\..\..\DEPENDENCIES\libxml2\installed\bin;..\..\..\DEPENDENCIES\gsl-1.8\bin

evolab-devel.exe -i ../model.xml -d 0.01 -b 0.01 -m 0.01 -M 0.01 -x 0.01 -X 0.02 -c 0.01 -r run-0 -n 6 -o out -D
