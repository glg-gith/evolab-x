/*
 * LabPluginsManager.h
 *
 *  Created on: 17 juil. 2012
 *      Author: golgauth
 */

#ifndef LABPLUGINSMANAGER_H_
#define LABPLUGINSMANAGER_H_


#include "Elp.h"

#include <Singleton.h>

#include <string>


#ifdef _RUN_UNDER_ECLIPSE
#define PLUGINS_ROOT 	./Debug/Plugins/
#define PLUGINS_MK	 	./Debug/Plugins/Makefile
#define PLUGINS_MK_TEM 	./Debug/Plugins/Makefile.template
#define PLUGINS_SO 		./Debug/Plugins/lib/libplugins.so
#define PLUGINS_DEP_SO 	./Debug/Plugins/lib/libelp.so
#define PLUGINS_SRC 	./Debug/Plugins/src/
#define PLUGINS_OBJ 	./Debug/Plugins/obj/
#define PLUGINS_TEM 	./Debug/Plugins/src/plugin.cpp.template
#else
#define PLUGINS_ROOT 	./Plugins/
#define PLUGINS_MK	 	./Plugins/Makefile
#define PLUGINS_MK_TEM 	./Plugins/Makefile.template
#define PLUGINS_SO 		./Plugins/lib/libplugins.so
#define PLUGINS_DEP_SO 	./Plugins/lib/libelp.so
#define PLUGINS_SRC 	./Plugins/src/
#define PLUGINS_OBJ 	./Plugins/obj/
#define PLUGINS_TEM 	./Plugins/src/plugin.cpp.template
#endif

using namespace std;

namespace elp {

class LabPluginsManager : public Singleton<LabPluginsManager> {
	friend class Singleton<LabPluginsManager>;
private:
	LabPluginsManager();
	~LabPluginsManager();

public:
	// TODO : return error codes...
	vector<string> GetPluginsFiles();
	bool PluginExists(string plug_name);
	void BuildPluginsLib();
	void CleanPluginsLib();
	void LoadPluginTemplate();
	void CreatePlugin(string of_name, string inner_code);
	void DeletePlugin(string of_name);
	void ExecutePlugin(string plugin_name, bool show_time);

public:
	bool ready;
	bool silent;
};

} /* namespace elp */
#endif /* LABPLUGINSMANAGER_H_ */


