import sys
sys.path.append('__build__')
import cyfwk

class PyAlg(cyfwk.AlgBase):
    def run(self):
        print '[py]  PyAlg.run()'

print "===> testing a py-alg"
s = PyAlg()
print "---> calling doRun()"
print s.doRun()
print "---> calling doJump()"
print s.doJump(3)
