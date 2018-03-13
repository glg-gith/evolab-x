export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:"../lib"
export PYTHONPATH=$PYTHONPATH:"."
python '../src/Spatial/BdmLsim2.py' -i model.xml -d 0.01 -b 0.01 -m 0.01 -M 0.01 -x 0.01 -X 0.02 -c 0.01 -r run-0 -n 6 -o out -D
