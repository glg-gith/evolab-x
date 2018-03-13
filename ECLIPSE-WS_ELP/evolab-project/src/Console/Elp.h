/*
 * Elp.h
 *
 *  Created on: 4 juil. 2012
 *      Author: golgauth
 */

#ifndef ELP_H_
#define ELP_H_

/** EVOLAB-X version used all over the program */
#define ELP_VERSION 0.0.3

// Comment this line for usage external to eclipse
////#define _RUN_UNDER_ECLIPSE

/*
 * This is how one gets a macro into quotes; an important one to keep
 * in all program templates.
 */
#define _QUOTEME(x) #x
#define QUOTEME(x) _QUOTEME(x)


#if defined(_UNICODE)
  #if !defined(_T)
    #define _T(x) L##x
  #endif // not defined _T
#else
  #ifndef _T
  #define _T(x) x
  #endif
#endif


#include "LabUtils.h"
#include "LabPluginsManager.h"
#include "LabLibLoader.h"
#include "LabConsole.h"
#include "LabProcessor.h"
#include "LabVarManager.h"
#include "LabEventManager.h"

#include "Singleton.h"

namespace elp {


}


#endif /* ELP_H_ */
