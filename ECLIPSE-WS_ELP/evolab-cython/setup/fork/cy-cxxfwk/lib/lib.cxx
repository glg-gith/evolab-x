#include "Python.h"
#include "lib.h"

#include "cyfwk_api.h"

#include <iostream>

IAlg::~IAlg()
{
}

IAlg::IAlg()
{
    std::cout << "[c+]  IAlg::IAlg()\n";
}

void
IAlg::doRun()
{
  std::cout << "[c+]  IAlg::doRun()\n";
  run();
}

void
IAlg::doJump(int i)
{
  std::cout << "[c+]  IAlg::doJump(" << i << ")\n";
  jump(i);
}

void
IAlg::jump(int i)
{
  std::cout << "[c+]  IAlg::jump(" << i << ")\n";
}

class Alg : public IAlg
{
public:
  Alg();
  ~Alg();

  void run();
  void jump(int i);
};

Alg::Alg()
{
  std::cout << "[c+]  Alg::Alg()\n";
}

Alg::~Alg()
{
  std::cout << "[c+]  Alg::~Alg()\n";
}

void
Alg::run()
{
  std::cout << "[c+]  Alg::run()\n";
}

void
Alg::jump(int i)
{
  std::cout << "[c+]  Alg::jump(" << i << ")\n";
}

IAlg *create_alg(int algid)
{
  switch (algid) {
  case 0:
    return new Alg;
  case 1:
    return 0;
  default:
    return 0;
  }

  return 0;
}

CyAlgBase::CyAlgBase(PyObject *obj) :
  m_obj(obj)
{
  if (import_cyfwk()) {
    std::cerr << "[c+]  error in import_cyfwk!\n";
  } else {
    Py_XINCREF(this->m_obj);
  }
}

CyAlgBase::~CyAlgBase()
{
  Py_XDECREF(this->m_obj);
}

void
CyAlgBase::run()
{
  if (this->m_obj) {
    std::cout << "[c+]  CyAlgBase::run()\n";
    int error;
    //cy_call_fct(this->m_obj, "run", &error);
    cy_call_run(this->m_obj, &error);
    if (error)
      std::cerr << "** you must override 'run', it is a pure virtual function\n";
    return;
  }
  std::cerr << "** invalid cy-state: obj [" << this->m_obj << "]\n";
  return;
}

void
CyAlgBase::jump(int i)
{
  std::cout << "[c+]  CyAlgBase::jump(" << i << ")\n";
  if (this->m_obj) {
    int error;
    // call a virtual overload, if it exists
    cy_call_jump(this->m_obj, i, &error);
    if (error)
      // call parent method
      IAlg::jump(i);
    return;
  }
  std::cerr << "** invalid cy-state: obj [" << this->m_obj << "]\n";
  return;
}
