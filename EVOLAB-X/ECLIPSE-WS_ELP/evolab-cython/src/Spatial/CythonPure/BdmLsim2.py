import sys

import pstats, cProfile
#import pyximport
#pyximport.install()

from bdmlsim2 import PyExecClass



#@main
def main():
    ex = PyExecClass()
    ex.Start()
    
#    cProfile.runctx("ex.Start()", globals(), locals(), "Profile.prof")
#    
#    s = pstats.Stats("Profile.prof")
#    s.strip_dirs().sort_stats("time").print_stats()

#@run
if __name__ == "__main__":
    sys.exit(main())


