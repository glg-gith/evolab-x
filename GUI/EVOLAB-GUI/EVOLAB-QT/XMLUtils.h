#ifndef XMLUTILS_H
#define XMLUTILS_H

#include <libxml/parser.h>
#include <libxml/encoding.h>
#include <libxml/tree.h>

#include <iostream>
#include <list>

#include <string>
#include <vector>
#include <algorithm>


//#include "mainwindow.h"
#include "Consts.h"
#include "Models/PointerLessModels.h"

#include <LabXMLUtils.h>


using namespace elps;

class XMLUtils
{
public:
    XMLUtils();


//    string networkFile;
//    string currentDist, defaultDist;
//    unsigned int currentNbNodes, defaultNbNodes;
//    unsigned int currentDistId, defaultDistId;

//    std::vector<t_dist_struct> *vec_dist;

    PointerLessModels::t_network_items_list *p_default_net_params;

    ///t_default_pop_params *vec_default_pop_params;
    PointerLessModels::t_cells_group *p_default_pop_params;

    ////

    // Load all Network params
//    std::vector<t_dist_struct> *LoadNetworkParams(std::string filename = "", bool skip_general_params = false);
//    void SaveNetworkParams(string filename);
    PointerLessModels::t_network_items_list *LoadNetworkParams(std::string filename = "", bool skip_general_params = false);
    xmlNodePtr SaveNetworkParams(string filename, PointerLessModels::t_network_items_list *p_nil);


    // Load all pops params
    xmlNodePtr SavePopParams(string filename, std::vector<PointerLessModels::t_cells_group> *pops_vec);
    PointerLessModels::t_cells_group *LoadDefaultPopParams();
    std::vector<PointerLessModels::t_cells_group> *LoadPopParams(std::string filename = "");

    // Load all vars params
    ///xmlNodePtr SavePopParams(string filename, std::vector<PointerLessModels::t_cells_group> *pops_vec);
    PointerLessModels::t_vars *LoadStdMathVars(std::string filename = XML_STD_CMATH_VARS_FILE);
    PointerLessModels::t_vars *LoadCustomTemplateVars(std::string filename = XML_CUSTOM_TEMPLATE_VARS_FILE);
    ///PointerLessModels::t_vars *LoadCustomVars(std::string filename = "");

};

#endif // XMLUTILS_H
