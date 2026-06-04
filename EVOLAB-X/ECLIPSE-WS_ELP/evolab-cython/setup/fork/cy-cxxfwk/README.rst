cy-cxxfwk
=========

A little example on how to have a cython class blend in into a C++ framework and python module

Demonstrates the following:

- Creating a cython extension for a c++ class
- Overriding c++ virtual and pure virtual methods in a cython extension
- Utilizing a python subclass of that extension in a python module
- Utilizing a cython subclass of that extension in a c++ application that starts a python interpreter

to test:
--------

::

  ./waf configure clean build && (cd __build__; PYTHON=`which python` LD_LIBRARY_PATH=`pwd` PYTHONPATH=.:${PYTHONPATH} ./test-app) && python py/test.py



you should see something like this:

::

    === app ===
    :: python initialized
    [cy]  cyfwk module imported
    :: import_cyfwk ok
    ===> testing a pure c++ alg...
    [c+]  IAlg::IAlg()
    [c+]  Alg::Alg()
    ---> calling doRun()
    [c+]  IAlg::doRun()
    [c+]  Alg::run()
    [c+]  Alg::~Alg()
    ===> testing a cy-alg...
    [cy]  MyAlg.__init__()
    [cy]  AlgBase.__init__
    [c+]  IAlg::IAlg()
    ---> calling doRun()
    [c+]  IAlg::doRun()
    [c+]  CyAlgBase::run()
    [cy]  cy_call_run()
    [cy]  MyAlg.run() => 42
    ---> calling doJump()
    [c+]  IAlg::doJump(2)
    [c+]  CyAlgBase::jump(2)
    [cy]  cy_call_jump(2)
    [c+]  IAlg::jump(2)
    === bye.
    [cy]  cyfwk module imported
    ===> testing a py-alg
    [cy]  AlgBase.__init__
    [c+]  IAlg::IAlg()
    ---> calling doRun()
    [cy]  AlgBase.doRun()
    [c+]  IAlg::doRun()
    [c+]  CyAlgBase::run()
    [cy]  cy_call_run()
    [py]  PyAlg.run()
    None
    ---> calling doJump()
    [cy]  AlgBase.doJump(3)
    [c+]  IAlg::doJump(3)
    [c+]  CyAlgBase::jump(3)
    [cy]  cy_call_jump(3)
    [c+]  IAlg::jump(3)

