/*
 * LabPluginsManager.cpp
 *
 *  Created on: 17 juil. 2012
 *      Author: golgauth
 */

#include "Elp.h"

#include <stdlib.h>
#include <stdio.h>
#include <fstream>

namespace elp {

LabPluginsManager::LabPluginsManager() {
	ready = false;
	silent = false;
}

LabPluginsManager::~LabPluginsManager() {
}


vector<string> LabPluginsManager::GetPluginsFiles()
{
	string dir = string(QUOTEME(PLUGINS_SRC));
	vector<string> src = vector<string>();
	LabUtils::ListDir(dir, src, true, ".cpp");
	return src;
}

bool LabPluginsManager::PluginExists(string plug_name)
{
	vector<string> names = GetPluginsFiles();
	for (vector<string>::iterator it = names.begin(); it<names.end(); it++)
		if (*it == plug_name + ".cpp") { return true; }
	return false;
}


void LabPluginsManager::BuildPluginsLib()
{
	try {

		// Clean lib
		remove(QUOTEME(PLUGINS_SO));

		cout << "Plugins under process ... ";
		string s_quiet = (this->silent) ? " -s " : "";


		// Get plugins filenames
		vector<string> src = LabPluginsManager::GetPluginsFiles();
		vector<string> obj = vector<string>();

		string s_src ="", s_obj = "";
		for (unsigned int i = 0;i < src.size();i++) {
			obj.push_back(src[i].substr(0, src[i].find_last_of(".")) + ".o");
			s_obj += obj[i] + " ";
		}

		// Read template makefile
		string n_mk_template = QUOTEME(PLUGINS_MK_TEM); //prefix + "Plugins/Makefile.template";
		string content = LabUtils::FileToString(n_mk_template);

		// Set plugins output objects files & create makefile
		content = LabUtils::StringReplace(content, "<//MODULES//>", s_obj);
		string n_mk = QUOTEME(PLUGINS_MK); //prefix + "Plugins/Makefile";
		LabUtils::StringToFile(n_mk, content);

		// Make the plugins shared library
		string mk_cmd = "make --directory " + (string)QUOTEME(PLUGINS_ROOT) + s_quiet; //prefix + "Plugins/";
		system(mk_cmd.c_str());

		ifstream lib(QUOTEME(PLUGINS_SO));
		if (lib) {
			cout << LabUtils::ColorString("Library \"" + (string)QUOTEME(PLUGINS_SO) + "\" DONE !", 34) << endl;
			this->ready = true;
		}
		else {
			cout << LabUtils::ColorString("Library \"" + (string)QUOTEME(PLUGINS_SO) + "\" FAILED !", 31) << endl;
			this->ready = false;
		}

	} catch (...) {
		// Keep going
	}
}


void LabPluginsManager::CleanPluginsLib()
{
	string s_quiet = (this->silent) ? " -s " : "";

	// Make clean plugins
	try {
		string mk_cmd = "make --directory " + (string)QUOTEME(PLUGINS_ROOT) + " clean" + s_quiet;
		system(mk_cmd.c_str());
	} catch (...) {
		// Keep going
	}
}

void LabPluginsManager::CreatePlugin(string plugin_name, string inner_code)
{

	// Read template plugin file
	string n_plug_template = QUOTEME(PLUGINS_TEM);
	string content = LabUtils::FileToString(n_plug_template);

	// Set plugins output objects files & create makefile
	content = LabUtils::StringReplace(content, "<//PLUGINNAME//>", plugin_name);
	content = LabUtils::StringReplace(content, "<//INNERCODE//>", inner_code);
	LabUtils::StringToFile((string)QUOTEME(PLUGINS_SRC) + plugin_name + ".cpp", content);

}

void LabPluginsManager::DeletePlugin(string plugin_name)
{
	string plug_src = (string)QUOTEME(PLUGINS_SRC) + plugin_name + ".cpp";
	string plug_obj = (string)QUOTEME(PLUGINS_OBJ) + plugin_name + ".o";
	remove(plug_src.c_str());
	remove(plug_obj.c_str());
}

void LabPluginsManager::ExecutePlugin(string plugin_name, bool show_time)
{
	typedef void (*ret_t)(bool);

	try {
		if (this->ready)
		{
			LabLibLoader *my_lib = new LabLibLoader(QUOTEME(PLUGINS_SO), QUOTEME(PLUGINS_DEP_SO));
			//cout << "Loaded."  << endl;
			ret_t exec_func = (ret_t) (my_lib->GetFunc(plugin_name.c_str()));
			exec_func(show_time);
			delete my_lib;
			//cout << "Unloaded." << endl;
		}
	} catch (char* e) {
		cout << e << endl;
	}
}


} /* namespace elp */






