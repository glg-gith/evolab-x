#include "Python.h"
#include "lib.h"
#include "cyfwk_api.h"

#include <iostream>

void print_pyerr()
{
  // fetch error
  PyObject* pytype = 0, *pyvalue = 0, *pytrace = 0;
  PyErr_Fetch (&pytype, &pyvalue, &pytrace);
  Py_XINCREF  (pytype);
  Py_XINCREF  (pyvalue);
  Py_XINCREF  (pytrace);
  // restore...
  PyErr_Restore (pytype, pyvalue, pytrace);
  // and print
  PyErr_Print();
}

int main(int argc, char **argv) 
{
  std::cout << "=== app ===\n";
  Py_Initialize();
  if (!Py_IsInitialized()) {
    std::cerr << ":: could not initialize python interpreter !\n";
    print_pyerr();
    return 1;
  } else {
    std::cout << ":: python initialized\n";
  }
  PyEval_InitThreads();
  if (!PyEval_ThreadsInitialized()) {
    std::cerr << ":: could not init GIL !\n";
    print_pyerr();
    return 1;
  }

  {
    if (import_cyfwk()) {
      std::cerr << ":: error in import_cyfwk!\n";
      print_pyerr();
      return 1;
    } else {
      std::cout << ":: import_cyfwk ok\n";
    }
  }
  std::cout << "===> testing a pure c++ alg...\n";
  {
    IAlg *alg = create_alg(0);
    std::cout << "---> calling doRun()\n";
    alg->doRun();
    delete alg;
  }

  std::cout << "===> testing a cy-alg...\n";
  {
    IAlg *alg = cy_create_alg();
    if (alg) {
      std::cout << "---> calling doRun()\n";
      alg->doRun();
      std::cout << "---> calling doJump()\n";
      alg->doJump(2);
    }
    else {
      std::cerr << "::: nullptr!\n";
    }
    delete alg;
  }

  std::cout << "=== bye.\n";
  return 0;
}
