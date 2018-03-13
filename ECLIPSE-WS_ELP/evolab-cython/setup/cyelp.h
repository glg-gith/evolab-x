#ifndef __PYX_HAVE__cyelp
#define __PYX_HAVE__cyelp


/* "/home/golgauth/Documents/02_DEV/EVOLAB/ECLIPSE-WS_ELP/evolab-cython/setup/pyext/LabSite.pyx":11
 * '''
 * # Redefine 't_output' enum type (no other way to do ??)
 * cdef public enum t_deptype:             # <<<<<<<<<<<<<<
 *     #: Dependency level filter for 'INDIVIDUAL' attributes
 *     IND_DEP  = 1
 */
enum t_deptype {
  IND_DEP = 1,
  SITE_DEP = 2,
  ENV_DEP = 4
};

/* "/home/golgauth/Documents/02_DEV/EVOLAB/ECLIPSE-WS_ELP/evolab-cython/setup/pyext/LabOutputer.pyx":9
 * 
 * # Redefine 't_output' enum type (no other way to do ??)
 * cdef public enum t_output:             # <<<<<<<<<<<<<<
 *     TO_FILE = 1
 *     TO_STD  = 2
 */
enum t_output {

  /* "/home/golgauth/Documents/02_DEV/EVOLAB/ECLIPSE-WS_ELP/evolab-cython/setup/pyext/LabOutputer.pyx":12
 *     TO_FILE = 1
 *     TO_STD  = 2
 *     TO_BOTH = TO_FILE | TO_STD             # <<<<<<<<<<<<<<
 * 
 * 
 */
  TO_FILE = 1,
  TO_STD = 2,
  TO_BOTH = (TO_FILE | TO_STD)
};

#ifndef __PYX_HAVE_API__cyelp

#ifndef __PYX_EXTERN_C
  #ifdef __cplusplus
    #define __PYX_EXTERN_C extern "C"
  #else
    #define __PYX_EXTERN_C extern
  #endif
#endif

__PYX_EXTERN_C DL_IMPORT(void) cy_call_func_void(PyObject *, char *, int *);
__PYX_EXTERN_C DL_IMPORT(int) cy_call_func_bool(PyObject *, char *, int *);
__PYX_EXTERN_C DL_IMPORT(int) cy_call_func_int(PyObject *, char *, int *);
__PYX_EXTERN_C DL_IMPORT(double) cy_call_func_double(PyObject *, char *, int *);

#endif /* !__PYX_HAVE_API__cyelp */

#if PY_MAJOR_VERSION < 3
PyMODINIT_FUNC initcyelp(void);
#else
PyMODINIT_FUNC PyInit_cyelp(void);
#endif

#endif /* !__PYX_HAVE__cyelp */
