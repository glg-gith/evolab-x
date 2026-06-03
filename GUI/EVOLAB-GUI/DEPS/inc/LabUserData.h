/*
 * LabUserData.h
 *
 *  Created on: 10 oct. 2012
 *      Author: golgauth
 */

#ifndef LABUSERDATA_H_
#define LABUSERDATA_H_


//#include <set>

#include <LabConsts.h>


//// Cython !!!
#ifndef NOPYTHON
	#include <Python.h>
	//#include <LabCythonUtils.h>
#endif

namespace elps {

/**
 * This is the base class for all the classes used for describing a Site.
 * A Site can be completely user defined (by using LabUserData, a custom
 * subclass of it, or one of the ready-made ones - See LabSiteBase, ...).
 *
 * The related user specific information can be stored using LabUserData::SetData()
 * and / or by extending LabUserData class (adding members).
 *
 */
class LIB_CLASS LabUserData {
public:

	LabUserData();


	//// Cython !!!
#ifndef NOPYTHON
	/**
	 * Cython usage only : DO NOT USE !
	 * @param obj The PyLabUserData Python object related to this
	 */
	LabUserData(PyObject *obj);
	/**
	 * Cython usage only : DO NOT USE !
	 * @param obj The PyLabUserData Python object related to this
	 * @param data The additional data Python object related to this (facult.)
	 */
	LabUserData(PyObject *obj, PyObject *data);
#endif


	virtual ~LabUserData();


	//	virtual LabUserData *New(LabNode *parent = NULL) { return NULL; } //= 0;		// Should be pure virtual but Cython doesn't like it
	//	virtual void Copy(LabUserData *src_site /*, int dep_filter = 0*/) { } //= 0;	// Should be pure virtual but Cython doesn't like it

	/* Sets stuffs */
	//void Set(bool s) { this->isSet = s; };
	/**
	 * Get the Site index (unique id in the whole population).
	 * @return The index.
	 */
	int  GetId();
	/**
	 * Assign an id to this Site.
	 * @param id The index.
	 */
	void SetId(unsigned int id);


//	/**
//	 * Cython utility. DO NOT USE !<br/>
//	 * Get the Python object representation of this Site.
//	 */
//	inline void *GetCyPyObj() { return cy_py_obj_; }
//	/**
//	 * Cython utility. DO NOT USE !<br/>
//	 * Set the Python object representation of this Site.
//	 * @param cy_py_obj The Python object
//	 */
//	inline void SetCyPyObj(void *cy_py_obj) { cy_py_obj_ = cy_py_obj; }
//	/**
//	 * Cython utility. DO NOT USE !<br/>
//	 * Get the Python object representation of the data carried by this.
//	 */
//	inline void *GetCyData() { return cy_data_; }
//	/**
//	 * Cython utility. DO NOT USE !<br/>
//	 * Set the Python object representation of the data carried by this.
//	 * @param cy_data The Python object carried
//	 */
//	inline void SetCyData(void *cy_data) { cy_data_ = cy_data; }

	//// Cython !!!
#ifndef NOPYTHON

	PyObject * CyGetPyObj();
	void CySetPyObj(PyObject *obj);

	PyObject * CyGetPyData();
	void CySetPyData(PyObject *obj);

#endif


protected:

private:

//	// The Python object
//	void *cy_py_obj_;
//	// The Python object carried
//	void *cy_data_;

	//// Cython !!!
	bool _is_cython;
#ifndef NOPYTHON
	PyObject *m_obj;
	PyObject *m_data;
#endif


	//bool isSet;

	// The unid
	unsigned int id;



};

} /* namespace elps */
#endif /* LABUSERDATA_H_ */
