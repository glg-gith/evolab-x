#ifndef CONSTS_H
#define CONSTS_H


#include <QDebug>
#include <LabConsts.h>


// Misc
#define EVOLAB_GUI_VERSION 0.0.3

#define MACRO_VARIABLES_TITLE "Variable / Function Formula"
#define MACRO_LIFECYCLE_TITLE "Rate / Transition Formula"
//#define MACRO_VARIABLES_TITLE "Formula:          (Variable: \"<itemname>\")"
//#define MACRO_LIFECYCLE_TITLE "Rate / Transition Formula:          (Variable: \"<itemname>\")"
//#define MACRO_REPLACEMENT_TAG "<itemname>"

// Tabs
#define TAB_ID_POPULATIONS              0
#define TAB_ID_VARIABLES                -2 // DEPREC
#define TAB_ID_LIFE_CYCLE               1
#define TAB_ID_NETWORK                  2
#define TAB_ID_OUTPUTS                  3


// Paths
#define TEMP_PROJECT_PATH               "../TEMP/"

#define XML_DEFAULT_NETWORK_PATH        "../XML/Presets/Network/"
#define XML_DEFAULT_NETWORK_FILE        "../XML/Presets/Network/network.xml"
#define NETWORK_FILENAME                "network.xml"

#define XML_DEFAULT_POP_DESCRIPTOR_PATH "../XML/Presets/Populations/"
#define XML_DEFAULT_POP_DESCRIPTOR_FILE "../XML/Presets/Populations/populations.xml"
#define POPULATIONS_FILENAME            "populations.xml"

#define XML_DEFAULT_VARIABLES_PATH      "../XML/Presets/Variables/"
#define XML_STD_CMATH_VARS_FILE         "../XML/Presets/Variables/cmath-standard.xml"
#define XML_ELP_BUILTIN_VARS_FILE       "../XML/Presets/Variables/elp-builtin.xml"
#define XML_CUSTOM_TEMPLATE_VARS_FILE   "../XML/Presets/Variables/custom-templates.xml"
//#define XML_USER_TEMP_VARS_FILE         "../TEMP/user-defined-variables.xml"
#define USER_VARS_FILENAME              "user-defined-variables.xml"


// Math
#define NB_DECIMALS     6
#define DOUBLE_MARGIN   0.0000001


#endif // CONSTS_H
