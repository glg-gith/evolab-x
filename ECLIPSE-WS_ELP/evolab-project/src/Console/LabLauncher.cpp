/*
 * LabLauncher.cpp
 *
 *  Created on: 4 juil. 2012
 *      Author: golgauth
 *
 *      Controller
 */


#include <stdlib.h>

#include "Elp.h"


using namespace elp;
using namespace std;

int main(int argc, char *argv[])
{
	try
	{
		//-------------------   Init & Start singletons   -------------------//

		// PluginsManager
		LabPluginsManager *pluginsManager = LabPluginsManager::GetInstance();
		pluginsManager->silent = true;				// Avoids compilation verbose
//		//pluginsManager->CleanPluginsLib();
//		pluginsManager->BuildPluginsLib();
//		pluginsManager->ExecutePlugin("hello2", true);
//
//		pluginsManager->CreatePlugin("plop", "cout << \"coucou !\" << endl;");
//		pluginsManager->BuildPluginsLib();
//		pluginsManager->ExecutePlugin("plop", true);
//
//		pluginsManager->CreatePlugin("plop2", "cout << \"coucou 2 !\" << endl;");
//		pluginsManager->BuildPluginsLib();
//		pluginsManager->ExecutePlugin("plop2", true);
//
//		pluginsManager->DeletePlugin("plop");


		// VarManager


		// EventManager

		// Console
		LabConsole *console = LabConsole::GetInstance();
		console->Start();

		return 0;

	}
	catch (char *s_err)
	{
		cout << "Error : " << s_err << '\n';
		exit(-1);
	}

}
