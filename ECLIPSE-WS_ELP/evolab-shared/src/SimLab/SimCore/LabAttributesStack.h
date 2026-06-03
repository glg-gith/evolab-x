#ifndef ELPS_LABATTRIBUTESSTACK_H
#define ELPS_LABATTRIBUTESSTACK_H


#include "LabIBinarizable.h"
#include <string>
using namespace std;
#include <vector>
using namespace std;

namespace elps {

//*
// * Stack of attributes that an individual can have in an individual centered
// * simulation context.<br/>
// * The attributes can be defined at 3 different levels, they can concern (t_deptype) :
// *
// * - The individual
// * - The site the individual lives in
// * - The global/environment context
// *
// * Aka : A LabAttributesStack is composed of 3 stacks which can be accessed using
// *       the methods GetIndAttributes(), GetSiteAttributes() and GetEnvAttributes().
// *
// * USAGE :
// *
// * - Instantiate
// * - Push attributes(s) (PushAttribute() function)
// * - The attribute value (int or double) can be updated at any time and filtered by
// * 		its dependency level (t_deptype)
// *
// * (in addition to events that can be fired at Site level. See LabEventStack class)
// *
// 
class LabAttributesStack : public LabIBinarizable {
  public:
    struct t_attr_data {
            //* Dependency level 
        ////t_deptype dep_type;
        //* Used for storage/access to double value without any cast 
        
        double d_value;

        // Removed for simplicity (easier for Cython interfacing, as well as for indexing features) :
        // /** Used for storage/access to integer value without any cast */
        // int i_value;
        // /** Used for storage/access to undefined typed value requiring a cast for later use */
        // void *o_value;
        //= false
        bool indexed;

        //= 1
        int nb_beans;

    };
    
    struct t_attr {
            //* Attribute name (better choose a unique one) 
        
        string name;

        //* Data/info related to the attribute 
        
        t_attr_data data;

    };
    

  private:
    // For accessing all the available attributes
    
    vector<t_attr> attributes;


  public:
    LabAttributesStack();

    virtual ~LabAttributesStack();

    //		t_attr() {};
    //		t_attr(const t_attr& rhs) {
    ////			cout << "t_attr copy ctor called !!!!!!!" << endl;
    //			name = rhs.name;
    //			data.d_value = rhs.data.d_value;
    //			data.indexed = rhs.data.indexed;
    //			data.nb_beans = rhs.data.nb_beans;
    //		}
    //*
    //	 * Add a new attribute on top of the stack.
    //	 * @param name Name of the attribute (better use a unique name)
    //	 * @param dep_type Dependency level
    //	 * @param d_value Value expressed as a double (use i_value if an integer value is required)
    //	 * @param i_value Value expressed as an int (use d_value if a float value is required)
    //	 
    void PushAttribute(string name, double d_value = 0.0, bool indexed = false, int nb_beans = 1				// 1 = boolean attribute
			);

    //*
    //	 * Remove the last pushed attribute from the stack.
    //	 * @param dep_type The dependency level has to be specified<br/>
    //	 * 			(as the stack deals with 3 separated vectors - one for each level)
    //	 
    t_attr PopAttribute();

    ////
    vector<t_attr> & GetAttributes();

    //	/**
    //	 * Get a list of the attributes related to the individual context.
    //	 * @return A list of attributes.
    //	 */
    //	vector<t_attr*> GetIndAttributes();
    //	/**
    //	 * Get a list of the attributes related to the site context.
    //	 * @return A list of attributes.
    //	 */
    //	vector<t_attr*> GetSiteAttributes();
    //	/**
    //	 * Get a list of the attributes related to the environment context.
    //	 * @return A list of attributes.
    //	 */
    //	vector<t_attr*> GetEnvAttributes();
    ////
    int GetSize();

    //	/**
    //	 * Get the number of attributes given a dependency level
    //	 * @param dep_type The dependency level
    //	 * @return The number attributes for this dependency level
    //	 */
    //	int GetSize(t_deptype dep_type);
    //	/**
    //	 * Cython compatible version (using int instead of t_deptype enum)
    //	 */
    //	int GetSize(int dep_type);
    bool IsValidIndexedAttrValue(int id, int i_value);

    //*
    //	 * Get a single attribute by its dependency level and its id
    //	 * @param dep_type Dependency level
    //	 * @param id Identifier in the list relatively to the specified dependency level
    //	 * @return
    //	 
    t_attr * GetAttribute(int id);

    //*
    //	 * Get a single attribute by its dependency level and its name
    //	 * @param dep_type Dependency level
    //	 * @param name Name in the list relatively to the specified dependency level
    //	 * @return
    //	 
    t_attr * GetAttribute(string name);

    //*
    //	 * Copy the attributes of a given stack (src_stack) into this one.
    //	 * @param src_stack Source stack
    //	 * @param dep_filter Filter used to select which attributes will be copied<br/>
    //	 * 			(ex1 : dep_filter = IND_DEP | SITE_DEP, means : copies only non-environmental attributes<br/>
    //	 * 			 ex2 : dep_filter = IND_DEP | SITE_DEP | ENV_DEP : copies all)
    //	 
    void Copy(LabAttributesStack * src_stack);

    ////void Copy(LabAttributesStack *src_stack, int dep_filter = IND_DEP | SITE_DEP | ENV_DEP);
    // Implement virtual
    //*
    //	 * Save the current state of the stack to a given output stream (binary format).
    //	 * @param ostream Output stream to backup to.
    //	 
    void Binarize(ofstream * ostream);

    //*
    //	 * Restore the stack from a given input stream (binary format).
    //	 * @param istream Input stream from which to get the info required to restore the stack
    //	 
    void UnBinarize(ifstream * istream);

};

} // namespace elps
#endif
