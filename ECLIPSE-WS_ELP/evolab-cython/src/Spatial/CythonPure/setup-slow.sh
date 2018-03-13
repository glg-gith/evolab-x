CC="gcc"   \
CXX="g++"   \
CFLAGS="-I../../../inc"   \
LDFLAGS="-L../../../lib"   \
    python setup-slow.py build_ext --inplace
