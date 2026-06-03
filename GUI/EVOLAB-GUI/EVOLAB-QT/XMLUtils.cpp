#include "XMLUtils.h"

XMLUtils::XMLUtils() {
    //this->defaultDistId = 0;
    //this->defaultNbNodes = 100;
    ///this->vec_dist = new std::vector<XMLUtils::t_dist_struct>();

    this->p_default_net_params = this->LoadNetworkParams();

    //this->LoadDefaultNetwork();

    ////this->LoadNetworkParams();

    this->p_default_pop_params = new PointerLessModels::t_cells_group();
}

PointerLessModels::t_network_items_list *XMLUtils::LoadNetworkParams(std::string filename, bool skip_general_params)
{

    std::string networkFile = (filename == string("")) ? XML_DEFAULT_NETWORK_FILE : filename;

    cerr << "Start loading params from file: " << networkFile << endl;

    PointerLessModels::t_network_items_list *p_nil = new PointerLessModels::t_network_items_list();

    // Read dist name and nb nodes
    xmlDocPtr doc;
    xmlNodePtr netwkNode, distsNode, paramsNode;
    vector<xmlNodePtr> distsNodes;
    std::vector<xmlNodePtr> params_vec;
    std::ostringstream ss;
    std::istringstream iss, iss2;

    //std::vector<XMLUtils::dist_struct> *dist_vec = new std::vector<XMLUtils::dist_struct>();
    ///this->vec_dist->clear();
    ///this->p_default_net_params->items.clear();

    doc = xmlParseFile(networkFile.c_str());

    if (doc == NULL) {
        cerr << "Invalide XML input document ! ('" << networkFile << "')" << endl;
        xmlFreeDoc(doc);
        return NULL;
    }
    netwkNode = xmlDocGetRootElement(doc);
    if (netwkNode == NULL) {
        cerr << "Empty XML document !" << endl;
        xmlFreeDoc(doc);
        return NULL;
    }

    if (!skip_general_params) {
        // Read default nb nodes
        iss.str((char *)xmlGetProp(netwkNode, (const xmlChar *)"nbnodes"));
        ///iss >> this->currentNbNodes;
        iss >> p_nil->nbnodes;
        //this->defaultNbNodes = this->currentNbNodes;
        // Read default dist
        iss2.str((char *)xmlGetProp(netwkNode, (const xmlChar *)"defaultdist"));
        ///iss2 >> this->currentDistId;
        iss2 >> p_nil->defaultdist;
        //this->defaultDistId = 0;
    }

    // Read distributions
    distsNode = LabXMLUtils::GetChildByName(netwkNode, "Distributions");
    if (distsNode == NULL) {
        cerr << "Missing 'Distributions' node !" << endl;
        xmlFreeDoc(doc);
        return NULL;
    }

    // Get all distributions
    distsNodes = LabXMLUtils::GetChildrenByName(distsNode, "Distribution");

    if (distsNodes.size() == 0) {
        cerr << "No 'Distribution' node available !" << endl;
        xmlFreeDoc(doc);
        return NULL;
    } else {
        //        std::vector<xmlNodePtr>::iterator it = distsNodes.begin();
        //        for(it=distsNodes.begin(); it != distsNodes.end(); ++it) {
        for(unsigned int i=0; i<distsNodes.size(); ++i) {

            PointerLessModels::t_dist_struct a_dist;
            // Read dist name and desc
            distsNode = distsNodes.at(i); //(*it); //LabXMLUtils::GetChildByName(root, "Distribution");
            ss.str(""); ss.clear();
            ss << xmlGetProp(distsNode, (const xmlChar *)"name");
            a_dist.name = ss.str();
            //
            ss.str(""); ss.clear();
            ss << xmlGetProp(distsNode, (const xmlChar *)"desc");
            a_dist.desc = ss.str();

            // Read dist params
            ///paramsNode = LabXMLUtils::GetChildByName(distsNode, "Params");
            ///params_vec = LabXMLUtils::GetChildrenByName(paramsNode, "Param");
            params_vec = LabXMLUtils::GetChildrenByName(distsNode, "Param");

            std::vector<PointerLessModels::t_param_struct> *params_vec_s = new  std::vector<PointerLessModels::t_param_struct>();
            PointerLessModels::t_param_struct a_param;
            for (unsigned int j=0; j<params_vec.size(); ++j)
            {
                // Read name, type, value and desc
                a_param.name = std::string((char *)xmlGetProp(params_vec[j], (const xmlChar *)"name"));
                a_param.type = std::string((char *)xmlGetProp(params_vec[j], (const xmlChar *)"type"));
                a_param.value = std::string((char *)xmlGetProp(params_vec[j], (const xmlChar *)"value"));
                a_param.desc = std::string((char *)xmlGetProp(params_vec[j], (const xmlChar *)"desc"));

                params_vec_s->push_back(a_param);

                cout << a_param.name << " | " << a_param.type << " | " << a_param.value << " | " << a_param.desc << endl;
            }

            a_dist.params = *(params_vec_s);
            //dist_vec->push_back(a_dist);
            ///this->vec_dist->push_back(a_dist);
            p_nil->items.push_back(a_dist);

            // The default
            //if (it == distsNodes.begin()) {
            ///if (i == this->currentDistId) {
            //            if (i == p_nil->defaultdist) {
            //                this->currentDist = a_dist.name;
            //                this->defaultDist = this->currentDist;
            //                this->defaultDistId = this->currentDistId;
            //            }
        }
    }

    xmlFreeDoc(doc);
    xmlCleanupParser();
    xmlMemoryDump();

    //this->st_dist = dist_vec;
    ///return this->vec_dist;
    return p_nil;
}

xmlNodePtr XMLUtils::SaveNetworkParams(std::string filename, PointerLessModels::t_network_items_list *p_nil)
{
    xmlNodePtr rootNode = NULL;

    if (filename != string("")) {
        std::string networkFile = filename;

        xmlDocPtr doc;
        xmlNodePtr netwkNode, distsNode, distNode, paramsNode, paramNode;

        // Begin doc (XML tree)
        LIBXML_TEST_VERSION;
        doc = xmlNewDoc((xmlChar *)"1.0");
        // Network node
        netwkNode = xmlNewNode(NULL, (xmlChar *)"Network");
        rootNode = netwkNode;
        xmlDocSetRootElement(doc, netwkNode);
        ///xmlNewProp(netwkNode, (xmlChar *)"nbnodes", (xmlChar *)std::to_string(this->currentNbNodes).c_str());
        ///xmlNewProp(netwkNode, (xmlChar *)"defaultdist", (xmlChar *)std::to_string(this->currentDistId).c_str());
        xmlNewProp(netwkNode, (xmlChar *)"nbnodes", (xmlChar *)std::to_string(p_nil->nbnodes).c_str());
        xmlNewProp(netwkNode, (xmlChar *)"defaultdist", (xmlChar *)std::to_string(p_nil->defaultdist).c_str());
        // Dists
        distsNode = xmlNewNode(NULL, (xmlChar *)"Distributions");
        xmlAddChild(netwkNode, distsNode);
        ///for (unsigned int i=0; i<this->vec_dist->size(); ++i) {
        for (unsigned int i=0; i < p_nil->items.size(); ++i) {
            // New dist
            distNode = xmlNewNode(NULL, (xmlChar *)"Distribution");
            xmlAddChild(distsNode, distNode);
            xmlNewProp(distNode, (xmlChar *)"name", (xmlChar *)p_nil->items.at(i).name.c_str());
            xmlNewProp(distNode, (xmlChar *)"desc", (xmlChar *)p_nil->items.at(i).desc.c_str());
            //            // Params
            //            paramsNode = xmlNewNode(NULL, (xmlChar *)"Params");
            //            xmlAddChild(distNode, paramsNode);
            //            for (unsigned int j=0; j<this->vec_dist->at(i).params.size(); ++j) {
            //                // New param
            //                paramNode =  xmlNewNode(NULL, (xmlChar *)"Param");
            //                xmlAddChild(paramsNode, paramNode);
            //                t_param_struct ps = this->vec_dist->at(i).params.at(j);
            //                xmlNewProp(paramNode, (xmlChar *)"name", (xmlChar *)ps.name.c_str());
            //                xmlNewProp(paramNode, (xmlChar *)"type", (xmlChar *)ps.type.c_str());
            //                xmlNewProp(paramNode, (xmlChar *)"value", (xmlChar *)ps.value.c_str());
            //                xmlNewProp(paramNode, (xmlChar *)"desc", (xmlChar *)ps.desc.c_str());
            //            }
            for (unsigned int j=0; j < p_nil->items.at(i).params.size(); ++j) {
                // New param
                paramNode =  xmlNewNode(NULL, (xmlChar *)"Param");
                ///xmlAddChild(paramsNode, paramNode);
                xmlAddChild(distNode, paramNode);
                ///t_param_struct ps = this->vec_dist->at(i).params.at(j);
                PointerLessModels::t_param_struct ps = p_nil->items.at(i).params.at(j);
                xmlNewProp(paramNode, (xmlChar *)"name",  (xmlChar *)ps.name.c_str());
                xmlNewProp(paramNode, (xmlChar *)"type",  (xmlChar *)ps.type.c_str());
                xmlNewProp(paramNode, (xmlChar *)"value", (xmlChar *)ps.value.c_str());
                xmlNewProp(paramNode, (xmlChar *)"desc",  (xmlChar *)ps.desc.c_str());
            }
        }

        // Write doc (XML tree)
        xmlSaveFormatFileEnc(networkFile.c_str(), doc, "UTF-8", 1);
        xmlFreeDoc(doc);
        xmlCleanupParser();
        xmlMemoryDump();

    }

    return rootNode;
}


PointerLessModels::t_cells_group *XMLUtils::LoadDefaultPopParams()
{
    *(this->p_default_pop_params) = this->LoadPopParams()->at(0);
    return (this->p_default_pop_params);

    //    string descriptorFile = string(XML_DEFAULT_POP_DESCRIPTOR_FILE);

    //    // Read default pop stuffs
    //    xmlDocPtr doc;
    //    xmlNodePtr popNode, popItemNode, itemChildNode, paramsNode;
    //    vector<xmlNodePtr> params_vec;

    //    std::ostringstream ss;
    //    //std::istringstream iss, iss2;

    //    //std::vector<XMLUtils::dist_struct> *dist_vec = new std::vector<XMLUtils::dist_struct>();
    //    //this->st_dist->clear();

    //    doc = xmlParseFile(descriptorFile.c_str());

    //    if (doc == NULL) {
    //        cerr << "Invalide XML input document ! ('" << descriptorFile << "')" << endl;
    //        xmlFreeDoc(doc);
    //        return NULL;
    //    }

    //    popNode = xmlDocGetRootElement(doc);
    //    // Get pop default name
    //    this->vec_default_pop_params->pop_name = std::string((char *)xmlGetProp(popNode, (const xmlChar *)"name"));
    //    // Get pop default desc
    //    this->vec_default_pop_params->pop_desc = std::string((char *)xmlGetProp(popNode, (const xmlChar *)"desc"));

    //    popItemNode = LabXMLUtils::GetChildByName(popNode, "Item");
    //    if (popItemNode == NULL) {
    //        cerr << "Malformed XML document !" << endl;
    //        xmlFreeDoc(doc);
    //        return NULL;
    //    }

    //    // Read attribute(=0), states(=1), events(=2) defaults
    //    vector<PointerLessModels::t_basic_item> items_vec;
    //    // itemChildNode = popItemNode->children;
    //    for (itemChildNode = popItemNode->children; itemChildNode != NULL; itemChildNode = itemChildNode->next)
    //    {
    //        if (itemChildNode->type == XML_ELEMENT_NODE)
    //        {
    //            PointerLessModels::t_basic_item an_item;

    //            // Read name and desc
    //            ss.str(""); ss.clear();
    //            ss << xmlGetProp(itemChildNode, (const xmlChar *)"name");
    //            an_item.name = ss.str();
    //            //
    //            ss.str(""); ss.clear();
    //            ss << xmlGetProp(itemChildNode, (const xmlChar *)"desc");
    //            an_item.desc = ss.str();

    //            // Read dist params
    //            paramsNode = LabXMLUtils::GetChildByName(itemChildNode, "Params");
    //            params_vec = LabXMLUtils::GetChildrenByName(paramsNode, "Param");

    //            PointerLessModels::t_basic_param a_param;
    //            for (unsigned int j=0; j < params_vec.size(); ++j)
    //            {
    //                // Read name, type, value and desc
    //                a_param.name = std::string((char *)xmlGetProp(params_vec[j], (const xmlChar *)"name"));
    //                a_param.type = std::string((char *)xmlGetProp(params_vec[j], (const xmlChar *)"type"));
    //                a_param.value = std::string((char *)xmlGetProp(params_vec[j], (const xmlChar *)"value"));
    //                a_param.desc = std::string((char *)xmlGetProp(params_vec[j], (const xmlChar *)"desc"));

    //                an_item.params.push_back(a_param);

    //                cout << a_param.name << " | " << a_param.type << " | " << a_param.value << " | " << a_param.desc << endl;
    //            }

    //            items_vec.push_back(an_item);
    //            ////itemChildNode = itemChildNode->next;
    //        }
    //    }

    //    if (items_vec.size() != 3) {
    //        cerr << "Missing or bad information!" << endl;
    //        xmlFreeDoc(doc);
    //        return NULL;
    //    }

    //    this->vec_default_pop_params->attr = items_vec.at(0);
    //    this->vec_default_pop_params->state = items_vec.at(1);
    //    this->vec_default_pop_params->event = items_vec.at(2);

    //    xmlFreeDoc(doc);
    //    xmlCleanupParser();
    //    xmlMemoryDump();

    //    //this->st_dist = dist_vec;
    //    return this->vec_default_pop_params;

}



xmlNodePtr XMLUtils::SavePopParams(string filename, std::vector<PointerLessModels::t_cells_group> *pops_vec)
{
    xmlNodePtr rootNode = NULL;

    if (filename != string("")) {

        ///this->vec_pop_params->clear();

        ///this->networkFile = filename;

        xmlDocPtr doc;
        xmlNodePtr popsNode, popNode;
        xmlNodePtr attrsNode, statesNode, eventsNode, itemNode;
        xmlNodePtr paramsNode, paramNode;

        // Begin doc (XML tree)
        LIBXML_TEST_VERSION;
        doc = xmlNewDoc((xmlChar *)"1.0");

        // Populations node
        popsNode = xmlNewNode(NULL, (xmlChar *)"Populations");
        rootNode = popsNode;
        xmlDocSetRootElement(doc, popsNode);

        // Pop nodes
        for (unsigned int i=0; i < pops_vec->size(); ++i)
        {

            PointerLessModels::t_cells_group a_pop = pops_vec->at(i);
            // New Population
            popNode = xmlNewNode(NULL, (xmlChar *)"Population");
            xmlNewProp(popNode, (xmlChar *)"name", (xmlChar *)a_pop.name.c_str());
            xmlNewProp(popNode, (xmlChar *)"desc", (xmlChar *)a_pop.desc.c_str());
            xmlAddChild(popsNode, popNode);

            // Prepare for writing Attr, States and Events items
            attrsNode = xmlNewNode(NULL, (xmlChar *)"Attributes");
            xmlAddChild(popNode, attrsNode);
            statesNode = xmlNewNode(NULL, (xmlChar *)"States");
            xmlAddChild(popNode, statesNode);
            eventsNode = xmlNewNode(NULL, (xmlChar *)"Events");
            xmlAddChild(popNode, eventsNode);

            // For each cell
            for (unsigned int j=0; j < a_pop.cells.size(); ++j)
            {
                PointerLessModels::t_cell_items_list a_cell = a_pop.cells.at(j);
                //std::vector<PointerLessModels::t_basic_item> an_item_vec = a_cell.items.
                for (unsigned int k=0; k < a_cell.items.size(); ++k)
                {
                    PointerLessModels::t_basic_item an_item = a_cell.items.at(k);

                    switch ((int)a_cell.type) {
                    case 0:                                                     // Attributes cell
                        // New Attribute node
                        itemNode = xmlNewNode(NULL, (xmlChar *)"Attribute");
                        xmlAddChild(attrsNode, itemNode);
                        break;
                    case 1:                                                     // States cell
                        // New Attribute node
                        itemNode = xmlNewNode(NULL, (xmlChar *)"State");
                        xmlAddChild(statesNode, itemNode);
                        break;
                    case 2:                                                     // Events cell
                        // New Attribute node
                        itemNode = xmlNewNode(NULL, (xmlChar *)"Event");
                        xmlAddChild(eventsNode, itemNode);
                        break;
                    default:
                        cerr << "XMLUtils::SavePopParams(): Unrecognized cell type!" << endl;
                        break;
                    }

                    // Item xml attributes
                    xmlNewProp(itemNode, (xmlChar *)"name", (xmlChar *)an_item.name.c_str());
                    xmlNewProp(itemNode, (xmlChar *)"desc", (xmlChar *)an_item.desc.c_str());


                    // Item Param nodes
                    for (unsigned int m=0; m < an_item.params.size(); ++m)
                    {
                        // New Param node
                        paramNode =  xmlNewNode(NULL, (xmlChar *)"Param");
                        PointerLessModels::t_basic_param a_param = an_item.params.at(m);
                        xmlNewProp(paramNode, (xmlChar *)"name", (xmlChar *)a_param.name.c_str());
                        xmlNewProp(paramNode, (xmlChar *)"type", (xmlChar *)a_param.type.c_str());
                        xmlNewProp(paramNode, (xmlChar *)"value", (xmlChar *)a_param.value.c_str());
                        xmlNewProp(paramNode, (xmlChar *)"desc", (xmlChar *)a_param.desc.c_str());
                        xmlAddChild(itemNode, paramNode);
                    }
                }
            }

        }

        // Write doc (XML tree)
        xmlSaveFormatFileEnc(filename.c_str(), doc, "UTF-8", 1);
        xmlFreeDoc(doc);
        xmlCleanupParser();
        xmlMemoryDump();
    }

    return rootNode;
}

std::vector<PointerLessModels::t_cells_group> *XMLUtils::LoadPopParams(std::string filename)
{
    std::vector<PointerLessModels::t_cells_group> *pops_vec = new std::vector<PointerLessModels::t_cells_group>();

    string descriptorFile = string(XML_DEFAULT_POP_DESCRIPTOR_FILE);
    std::string xmlfilename;

    if (filename == std::string(""))
        xmlfilename = descriptorFile;
    else
        xmlfilename = filename;

    // Read pop stuffs
    xmlDocPtr doc;
    xmlNodePtr popsNode, popNode, itemNode, paramNode;
    vector<xmlNodePtr> popNodes;
    //    xmlNodePtr attrsNode, statesNode, eventsNode;
    //xmlNodePtr paramsNode, paramNode;

    std::ostringstream ss;

    doc = xmlParseFile(xmlfilename.c_str());

    if (doc == NULL) {
        cerr << "Invalide XML input document ! ('" << xmlfilename << "')" << endl;
        xmlFreeDoc(doc);
        return NULL;
    }

    popsNode = xmlDocGetRootElement(doc);
    //    // Get pop default name
    //    this->vec_default_pop_params->pop_name = std::string((char *)xmlGetProp(popNode, (const xmlChar *)"name"));
    //    // Get pop default desc
    //    this->vec_default_pop_params->pop_desc = std::string((char *)xmlGetProp(popNode, (const xmlChar *)"desc"));


    // Read Population nodes
    popNodes = LabXMLUtils::GetChildrenByName(popsNode, "Population");
    if (popNodes.size() == 0) {
        cerr << "No 'Population' descriptor node found!" << endl;
        xmlFreeDoc(doc);
        return NULL;
    }

    //std::vector<PointerLessModels::t_cell_items_list> cells;

    for (unsigned int i=0; i < popNodes.size(); i++)
    {
        PointerLessModels::t_cells_group a_pop;
        popNode = popNodes.at(i);

        // Read Pop name and desc
        ss.str(""); ss.clear();
        ss << xmlGetProp(popNode, (const xmlChar *)"name");
        a_pop.name = ss.str();
        //
        ss.str(""); ss.clear();
        ss << xmlGetProp(popNode, (const xmlChar *)"desc");
        a_pop.desc = ss.str();


        // Prepare cells for attrs, states, events...
        PointerLessModels::t_cell_items_list a_cil_1, a_cil_2, a_cil_3;
        a_cil_1.type = PointerLessModels::Attribute;
        a_cil_2.type = PointerLessModels::State;
        a_cil_3.type = PointerLessModels::Event;
        //
        a_pop.cells.push_back(a_cil_1);
        a_pop.cells.push_back(a_cil_2);
        a_pop.cells.push_back(a_cil_3);

        // Read cells

        //PointerLessModels::CellType type;
        ////PointerLessModels::CellType type = (PointerLessModels::CellType)0;
        int cell_type = 0;
        xmlNodePtr cellNode = popNode->xmlChildrenNode;  // First cell
        while (cellNode != NULL)
        {
            if (cellNode->type == XML_ELEMENT_NODE) {

                cerr << "> XML Cell: " << cellNode->name << endl;

                itemNode = cellNode->xmlChildrenNode;
                while (itemNode != NULL)
                {
                    if (itemNode->type == XML_ELEMENT_NODE) {

                        cerr << ">> XML Item: " << itemNode->name << endl;

                        PointerLessModels::t_basic_item an_item;
                        an_item.name = std::string((char *)xmlGetProp(itemNode, (const xmlChar *)"name"));
                        an_item.desc = std::string((char *)xmlGetProp(itemNode, (const xmlChar *)"desc"));

                        paramNode = itemNode->xmlChildrenNode;
                        while (paramNode != NULL)
                        {
                            if (paramNode->type == XML_ELEMENT_NODE) {

                                cerr << ">>> XML Param: " << paramNode->name << endl;

                                PointerLessModels::t_basic_param a_param;
                                a_param.name = std::string((char *)xmlGetProp(paramNode, (const xmlChar *)"name"));
                                a_param.type = std::string((char *)xmlGetProp(paramNode, (const xmlChar *)"type"));
                                a_param.value = std::string((char *)xmlGetProp(paramNode, (const xmlChar *)"value"));
                                a_param.desc = std::string((char *)xmlGetProp(paramNode, (const xmlChar *)"desc"));

                                // Add param to item
                                an_item.params.push_back(a_param);

                            }
                            // Goto next item (attr, state, event)
                            paramNode = paramNode->next;
                        }

                        // Add item to cell
                        //a_cil.items.push_back(an_item);
                        if (!xmlStrcmp(itemNode->name, (const xmlChar *)"Attribute")) {              // Is Attribute node
                            cell_type = (int)PointerLessModels::Attribute;
                            a_pop.cells.at(cell_type).items.push_back(an_item);
                        } else if (!xmlStrcmp(itemNode->name, (const xmlChar *)"State")) {           // Is State node
                            cell_type = (int)PointerLessModels::State;
                            a_pop.cells.at(cell_type).items.push_back(an_item);
                        } else if (!xmlStrcmp(itemNode->name, (const xmlChar *)"Event")) {           // Is Event node
                            cell_type = (int)PointerLessModels::Event;
                            a_pop.cells.at(cell_type).items.push_back(an_item);
                        }

                    }
                    // Goto next cell
                    itemNode = itemNode->next;

                }
            }
            // Goto next pop
            cellNode = cellNode->next;
        }

        // Append pop
        pops_vec->push_back(a_pop);
    }


    xmlFreeDoc(doc);
    xmlCleanupParser();
    xmlMemoryDump();

    return pops_vec;
}


PointerLessModels::t_vars *XMLUtils::LoadStdMathVars(std::string filename)
{
    PointerLessModels::t_vars *math_vars = new PointerLessModels::t_vars();

    std::string xmlfilename = filename;

    // Read pop stuffs
    xmlDocPtr doc;
    xmlNodePtr popsNode, popNode, itemNode, paramNode;
    vector<xmlNodePtr> popNodes;
    //    xmlNodePtr attrsNode, statesNode, eventsNode;
    //xmlNodePtr paramsNode, paramNode;

    std::ostringstream ss;

    doc = xmlParseFile(xmlfilename.c_str());

    if (doc == NULL) {
        cerr << "Invalide XML input document ! ('" << xmlfilename << "')" << endl;
        xmlFreeDoc(doc);
        return NULL;
    }

    popsNode = xmlDocGetRootElement(doc);
    //    // Get pop default name
    //    this->vec_default_pop_params->pop_name = std::string((char *)xmlGetProp(popNode, (const xmlChar *)"name"));
    //    // Get pop default desc
    //    this->vec_default_pop_params->pop_desc = std::string((char *)xmlGetProp(popNode, (const xmlChar *)"desc"));


    // Read Population nodes
    popNodes = LabXMLUtils::GetChildrenByName(popsNode, "Population");
    if (popNodes.size() == 0) {
        cerr << "No 'Population' descriptor node found!" << endl;
        xmlFreeDoc(doc);
        return NULL;
    }

    //std::vector<PointerLessModels::t_cell_items_list> cells;

    for (unsigned int i=0; i < popNodes.size(); i++)
    {
        PointerLessModels::t_cells_group a_pop;
        popNode = popNodes.at(i);
    }



    return math_vars;
}


PointerLessModels::t_vars *XMLUtils::LoadCustomTemplateVars(std::string filename)
{
    PointerLessModels::t_vars *template_vars = new PointerLessModels::t_vars();

    // TODO ...

    return template_vars;
}









