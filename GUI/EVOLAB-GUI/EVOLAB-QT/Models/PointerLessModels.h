#ifndef POINTERLESSMODELS_H
#define POINTERLESSMODELS_H

//#include <LabAttributesStack.h>
///#include <XMLUtils.h>
#include <libxml/parser.h>
#include <libxml/encoding.h>
#include <libxml/tree.h>

#include <iostream>
#include <vector>

#include "StringUtils.h"

namespace CTypes {
const std::string STR_VOID        = "VOID";
const std::string STR_VOID_       = "VOID_";
const std::string STR_BOOL        = "BOOL";
const std::string STR_INT         = "INT";
const std::string STR_DOUBLE      = "DOUBLE";
const std::string STR_INT_PTR     = "INT_PTR";
const std::string STR_DOUBLE_PTR  = "DOUBLE_PTR";
}


class PointerLessModels
{

public:


    typedef enum {
        Attribute,
        State,
        Event
    } CellType;

    typedef enum {
        Stochastic,
        Fixed
    } SpreadMethod;


    typedef struct {
        std::string name, desc;
    } t_basic_text;

    typedef struct {
        std::string name, desc;
        ///t_basic_text text;
        std::string type, value;
    } t_basic_param;

    typedef struct {
        std::string name, desc;
        ///t_basic_text text;
        std::vector<t_basic_param> params;
    } t_basic_item;

    typedef struct {
        ///std::string name, desc;
        ///t_basic_text text;
        CellType type;
        std::vector<t_basic_item> items;

        void addItem(CellType a_type, const t_basic_item& new_item) {
            //            qDebug("addItem:");
            //            qDebug(std::to_string(a_type).c_str());
            //            t_basic_item new_item;
            type = a_type;
            items.push_back(new_item);
            //            switch (a_type) {
            //            case 0:                 // Attr
            //                new_item.name = std::string("NewAttr");
            //                qDebug(new_item.name.c_str());
            //                items.push_back(new_item);
            //                qDebug("Done!");
            //                break;
            //            case 1:                 // State
            //                new_item.name = std::string("NewState");
            //                items.push_back(new_item);
            //                break;
            //            case 2:                 // Evt
            //                new_item.name = std::string("NewEvt");
            //                items.push_back(new_item);
            //                break;
            //            default:
            //                break;
            //            }

        }
    } t_cell_items_list;                        // t_pop_cell (_items_list)

    typedef struct {
        std::string name, desc;
        ///t_basic_text text;
        std::vector<t_cell_items_list> cells;
    } t_cells_group;                            // t_pop

    typedef struct {
        int nbnodes, defaultdist;
        std::vector<t_basic_item> items;

        void addItem(const t_basic_item& new_item) {
            items.push_back(new_item);
        }
    } t_network_items_list;                     // t_network (_items_list)


    // Semantic shortcuts
    typedef PointerLessModels::t_basic_item             t_dist_struct;
    typedef PointerLessModels::t_basic_param            t_param_struct;

    typedef PointerLessModels::t_cell_items_list        t_pop_cell;     //(_items_list)
    typedef PointerLessModels::t_cells_group            t_pop;
    typedef PointerLessModels::t_network_items_list     t_network;      //(_items_list)

    typedef PointerLessModels::t_basic_item             t_basic_var;

    // Meta model
    typedef struct {
        t_network network;
        std::vector<t_pop> pops;
        SpreadMethod spreadmethod;
        std::vector<double> spreadvalues;

        void addPop(const t_pop& pop, int index = -1, double spread_val = 0.0f) {
            if (index < 0) {
                pops.push_back(pop);
                spreadvalues.push_back(spread_val);
            } else {
                pops.insert(pops.begin() + index, pop);
                spreadvalues.insert(spreadvalues.begin() + index, spread_val);
            }
        }
        void removePop(/*const t_pop& pop,*/ int index = -1) {
            //            std::vector<t_pop>::iterator it1 = std::find(pops.begin(), pops.end(), pop);
            //            pops.erase(it1);
            //            std::vector<double>::iterator it2 = spreadvalues.begin() + (it1 - pops.begin();
            //            spreadvalues.erase(it2);
            if (index < 0) {
                pops.pop_back();
                spreadvalues.pop_back();
            } else {
                pops.erase(pops.begin() + index);
                spreadvalues.erase(spreadvalues.begin() + index);
            }
        }
    } t_meta_model;


    ///***** Variables editor *****///
    typedef enum {
        CMathBuiltin,
        ElpBuiltinKeyword,
        ElpBuiltin,
        Custom
    } VarType;

    typedef enum {
        RateCallback,
        EventCallback,
        UserDefined
    } MacroStyle;




    // "formula" can also be a simple function or var SIGNATURE
    // ex1: double FUNC(int var1, double var2)
    // ex2: double VAR = 1.0f
    // "insertionText()" returns:
    // ex1: "FUNC(var1,var2)
    // ex2: VAR
    // "infoText()" => See if we can have tooltip over words to display the signature
    // in the editor...
    typedef struct {
        //t_basic_text text;
        t_basic_var var;
        std::string formula;

        // Text
        std::string const_prefix = "DEF_CONST(";
        std::string infoText()      // Get signature
        {
            return formula.substr(0, formula.find_first_of("\n"));
        }
        std::string insertionText()
        {
            // Get signature from formula of selected Macro
            std::string signature = infoText();
            //// DEPREC... Remove parenthesis if return type starts with "CONST_" (CONST_INT or CONST_DOUBLE, for now...)
            ///
            if(signature.substr(0, const_prefix.size()) == const_prefix) {      // Is constant: Starts with "DEF_CONST("
                int comma_1 = signature.find(",", 0);
                int comma_2 = signature.find(",", comma_1 + 1);
                signature = signature.substr(comma_1+1, (comma_2-comma_1)-1);
            }
            else                                                    // Is function
            {
                // Remove return and params types
                ReplaceAll2(signature, CTypes::STR_BOOL, "");
                ReplaceAll2(signature, CTypes::STR_INT, "");
                ReplaceAll2(signature, CTypes::STR_DOUBLE, "");
                ReplaceAll2(signature, CTypes::STR_INT_PTR, "");
                ReplaceAll2(signature, CTypes::STR_DOUBLE_PTR, "");
            }
            // Remove spaces
            ReplaceAll2(signature, " ", "");

            return signature;
            // Insert resulting text at cursor position !!!
        }

        // Replacements
        // For const and func
        void changeType(std::string new_type)
        {
            //std::string signature = infoText();
            if (formula.substr(0, const_prefix.size()) == const_prefix)       // Is a constant
            {
                int comma_1 = formula.find("(", 0);
                int comma_2 = formula.find(",", comma_1+1);
                formula.replace(comma_1+1, (comma_2-comma_1)-1, new_type);
            }
            else                                                  // Function
            {
                formula.replace(0, formula.find_first_of(" "), new_type);
            }
        }
        // For func only
        void changeParamType(std::string new_type, int param_index)
        {
            if (param_index < 0) {
                std::clog << "Trying to change parameter that doesn't exists ?!" << std::endl;
                return;
            }

            if (formula.substr(0, const_prefix.size()) != const_prefix)       // Is a function
            {
                std::string signature = formula.substr(0, formula.find_first_of("\n")); //infoText();
                std::string new_signature = signature.substr(0, signature.find_first_of("(")+1);
                std::vector<std::string> params_vec;
                std::string delimiter = ",";
                std::clog << "Initial signature: " << signature << std::endl;

                // Get params part (between parenthesis)
                int par_1 = signature.find("(", 0);
                int par_2 = signature.find(")", par_1 + 1);
                std::string s = signature.substr(par_1+1, (par_2-par_1)-1);

                // Split params
                std::string token;
                int pos;
                while ((pos = s.find(delimiter)) != std::string::npos)
                {
                    token = s.substr(0, pos);
                    std::clog << token << std::endl;
                    //params_vec.push_back(ReplaceAll(token, " ", "")); // Mme Beigbeder-Louet
                    params_vec.push_back(Trim(token));

                    s.erase(0, pos + delimiter.length());
                }
                std::clog << s << std::endl;
                params_vec.push_back(Trim(s));

                // Change targetted param type
                if (param_index >= params_vec.size()) {
                    std::clog << "Trying to change parameter that doesn't exists ?!" << std::endl;
                    return;
                }

                std::string new_param_str = params_vec.at(param_index);
                std::clog << "Param before change: " << new_param_str << std::endl;
                new_param_str.erase(0, new_param_str.find_first_of(" "));
                new_param_str = new_type + new_param_str;
                std::clog << "Param after change: " << new_param_str << std::endl;

                // Rebuild signature
                for (unsigned int i=0; i < params_vec.size(); i++) {
                    if (i == param_index) { new_signature += new_param_str; }
                    else { new_signature += params_vec.at(i); }
                    new_signature += ", ";
                }
                std::clog << "New signature: " << new_signature << std::endl;
                // Remove trailing comma
                new_signature = new_signature.substr(0, new_signature.length()-2) + ")";
                std::clog << "New signature: " << new_signature << std::endl;

                // Replace signature in formula
                formula = formula.replace(0, formula.find_first_of(")")+1, new_signature);
                std::clog << "New formula: " << formula << std::endl;
            }
            else
                std::clog << "Trying to change parameter on a constant formula ?!" << std::endl;
        }

    } t_variable;

    // Addons for MACRO editor...
    typedef struct {
        std::string name = "Custom";
        std::string url;
        std::vector<t_variable> vars;
    } t_vars_category;

    typedef struct {
        VarType type = Custom;
        bool editable = true;
        std::string url;
        std::vector<t_vars_category> varscats;
    } t_vars;


    ///*** TODO ?? ***///
    template <typename T>
    static xmlNode *toXML(T model, std::string filename = 0) {
        xmlNode *node;
        //
        return node;
    }

    template <typename T>
    static T fromXML(xmlNode *xml_node, std::string filename = 0) {
        T model;
        //
        return model;
    }
};


#endif // POINTERLESSMODELS_H
