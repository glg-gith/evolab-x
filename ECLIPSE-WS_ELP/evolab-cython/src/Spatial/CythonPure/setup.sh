CC="gcc"   \
CXX="g++"   \
CFLAGS="-I../../../inc"   \
LDFLAGS="-L../../../lib"   \
    python setup.py build_ext --inplace
