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
        if (name.startswith("bdmlsim2") and not(name.endswith(".pyx"))):
            os.remove(os.path.join(root, name))
    for name in dirs:
        if (name == "build"):
            shutil.rmtree(name)

setup(
	cmdclass = {'build_ext': build_ext},
	ext_modules = [
	Extension("bdmlsim2", 
			  sources=[
#                        "../../../setup/cyelp.pyx", \
                        "bdmlsim2-slow.pyx", \
#                       "adapter/ATestClass.cpp", \
#                       "adapter/ALabSimulatorBase.cpp", \
#                       "adapter/ALabSimulatorTime.cpp", \
#                       "adapter/AValueArg.cpp", \
					   ],
			  libraries=["elp"],
			  language="c++",
#              extra_compile_args=["-fopenmp", "-O3"],
#              extra_link_args=[""]
			  )
	]
)						

#if __name__ == "__main__":
#    sys.exit(main())

