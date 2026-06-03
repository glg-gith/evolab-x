import sys
import os
import shutil

from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

#def main():
#    for arg in sys.argv:
#        if (arg == "clean"):

for root, dirs, files in os.walk(".", topdown=False):
    for name in files:
        if (name.startswith("cyelp") and not(name.endswith(".pyx"))):
            os.remove(os.path.join(root, name))
    for name in dirs:
        if (name == "build"):
            shutil.rmtree(name)

setup(
	cmdclass = {'build_ext': build_ext},
	ext_modules = [
	Extension("cyelp", 
			  sources=["cyelp.pyx", \
# 					   "adapter/ATestClass.cpp", \
#                        "adapter/ALabSimulatorBase.cpp", \
#                        "adapter/ALabSimulatorTime.cpp", \
#                        "adapter/ALabNetBinding.cpp", \
                       "adapter/AValueArg.cpp", \
#                        "adapter/ALabSiteSetsManager.cpp", \
#                        "adapter/ALabSite.cpp", \
					   ],
			  libraries=["elp"],
			  language="c++",
#              extra_compile_args=["-m64", "-msse4.2"] #, "-DCYTHON_REFNANNY"],
#              extra_compile_args=["-fopenmp", "-O3"],
#              extra_link_args=["-DCYTHON_REFNANNY"]
              extra_compile_args=['-fopenmp'], extra_link_args=['-fopenmp']
			  )
	]
)						

#if __name__ == "__main__":
#    sys.exit(main())

