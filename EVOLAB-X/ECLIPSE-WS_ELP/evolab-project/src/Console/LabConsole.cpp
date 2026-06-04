/*
 * LabConsole.cpp
 *
 *  Created on: 4 juil. 2012
 *      Author: golgauth
 */

#include "LabConsole.h"
#include <Singleton.h>

#include <typeinfo>



#if defined(_WIN32) && defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define CREATE_LEAKAGE_REPORT
#endif

#if defined( USINGDLL ) && defined( _WIN32 )
#error This sample can be used only with STATIC builds of muParser (on win32)
#endif

/** \brief This macro will enable mathematical constants like M_PI. */
#define _USE_MATH_DEFINES

#include <cstdlib>
#include <cstring>
#include <cmath>
#include <string>
#include <iostream>
#include <locale>
#include <limits>
#include <ios>
#include <iomanip>
#include <numeric>

#include <sstream>
#include <algorithm>
#include <iterator>

//#include "muParser.h"

#include "Elp.h"

#include <sys/time.h>

#define N_CONSOLE_PLUGIN console_temp

using namespace std;
//using namespace mu;
using namespace elp;


#if defined(CREATE_LEAKAGE_REPORT)

// Dumping memory leaks in the destructor of the static guard
// guarantees i won't get false positives from the ParserErrorMsg
// class wich is a singleton with a static instance.
struct DumpLeaks
{
	~DumpLeaks()
	{
		_CrtDumpMemoryLeaks();
	}
} static LeakDumper;

#endif



namespace elp {


//---------------------------------------------------------------------------
LabConsole::LabConsole() {
	cmdStack = "";
	pluginName = (string)QUOTEME(N_CONSOLE_PLUGIN);
}

LabConsole::~LabConsole() {
}


//---------------------------------------------------------------------------
void LabConsole::Splash()
{
#ifndef _RUN_UNDER_ECLIPSE
	ClearScreen();
#endif

	elp::console() << _T("\n");
	elp::console() << _T(" ___    _   _  _____  _      _____  ___           _    _    _ \n");
	elp::console() << _T("(  _`\\ ( ) ( )(  _  )( )    (  _  )(  _`\\        ( )  ( ) /' )\n");
	elp::console() << _T("| (_(_)| | | || ( ) || |    | (_) || (_) ) ______`\\`\\/'/'(_, |\n");
	elp::console() << _T("|  _)_ | | | || | | || |  _ |  _  ||  _ <'(______) >  <    | |\n");
	elp::console() << _T("| (_( )| \\_/ || (_) || |_( )| | | || (_) )        /'/\\`\\   | |\n");
	elp::console() << _T("(____/'`\\___/'(_____)(____/'(_) (_)(____/'       (_)  (_)  (_) \n");
	elp::console() << _T("                                                \n");
	elp::console() << _T("                                               \n");


	//elp::console() << _T("  Version ") << Parser().GetVersion(pviFULL) << _T("\n");
	elp::console() << _T("  Version = ") << (string)QUOTEME(ELP_VERSION) << _T("\n");
	elp::console() << _T("  (C) 2012 Sylvain Gandon - Sebastien Lion - CEFE - CNRS\n");
	elp::console() << _T("         (Development & Architecture - Gauthier Boaglio)\n");
}


//---------------------------------------------------------------------------
void LabConsole::Help()
{
	elp::console() << _T("---------------------------------------------------------------\n");
	elp::console() << _T("Commands:\n\n");
	elp::console() << _T("  $+ENTER         - closes and executes the current CL\n");
	elp::console() << _T("  quit            - exits the command line\n");
	elp::console() << _T("  cls             - clears the command line's display\n");
	elp::console() << _T("  reset           - clears all previous commands\n");
	elp::console() << _T("  listvar         - lists all defined variables\n");
	elp::console() << _T("  save(name)      - saves the current CL state as a plugin\n");
	elp::console() << _T("  run(name)       - runs a plugin (no name => runs CL content)\n");
	elp::console() << _T("  del(name)       - deletes a given plugin (WARN : definitively)\n");
	elp::console() << _T("  listplug        - lists all the available plugins\n");
	elp::console() << _T("  save2(path)     - saves the current CL state into a file\n");
	elp::console() << _T("  load(path)      - loads commands from a given batch file\n");
	elp::console() << _T("  print(var)      - displays the value of a variable\n");
	elp::console() << _T("  print2(var)     - idem, but will stand in the plugin code\n");
	elp::console() << _T("  load(path)      - loads commands from a given batch file\n");
	elp::console() << _T("---------------------------------------------------------------\n");
}


//---------------------------------------------------------------------------
void LabConsole::ClearScreen()
{
#ifdef _WIN32
	std::system ( "CLS" );
#else
	// Assume POSIX
	std::system ( "clear" );
#endif
}


//---------------------------------------------------------------------------
void LabConsole::ResetCmdStack()
{
	LabPluginsManager *pluginManager = LabPluginsManager::GetInstance();
	pluginManager->DeletePlugin(pluginName);
	pluginManager->BuildPluginsLib();
	cmdStack = "";
}

//---------------------------------------------------------------------------
void LabConsole::ListPlugins()
{
	LabPluginsManager *pluginManager = LabPluginsManager::GetInstance();
	vector<string> names = pluginManager->GetPluginsFiles();
	for (vector<string>::iterator it = names.begin(); it<names.end(); it++)
		elp::console() << LabUtils::ColorString((*it).substr(0, (*it).find_last_of(".")), 32) << endl;
}

//---------------------------------------------------------------------------
void LabConsole::Save(string plugin_name)
{
	LabPluginsManager *pluginManager = LabPluginsManager::GetInstance();
	pluginManager->CreatePlugin(plugin_name, cmdStack);
}

//---------------------------------------------------------------------------
void LabConsole::Run(string plugin_name)
{
	LabPluginsManager *pluginManager = LabPluginsManager::GetInstance();

	if (pluginManager->PluginExists(plugin_name))
	{
		pluginManager->BuildPluginsLib();
		if (pluginManager->ready)
		{
			// Pass the plugin to be processed to the LabProcessor
			LabProcessor *processor = LabProcessor::GetInstance();
			processor->ProcessPlugin(plugin_name);
		}
		else
		{
			elp::console() << LabUtils::ColorString("Plugin was not run !", 31) << endl;
		}
	}
	else
	{
		elp::console() << LabUtils::ColorString("Plugin doesn't exist !", 31) << endl;
		elp::console() << "Available plugins are :" << endl;
		ListPlugins();
	}
}


//---------------------------------------------------------------------------
void LabConsole::Delete(string plugin_name)
{
	LabPluginsManager *pluginManager = LabPluginsManager::GetInstance();
	if (pluginManager->PluginExists(plugin_name))
	{
		pluginManager->DeletePlugin(plugin_name);
	}
	else
	{
		elp::console() << LabUtils::ColorString("Plugin doesn't exist !", 31) << endl;
		elp::console() << "Available plugins are :" << endl;
		ListPlugins();
	}
}


//---------------------------------------------------------------------------
void LabConsole::Save2(string file_path)
{
	cout << "SAVED : " << file_path << endl;
	LabUtils::StringToFile(file_path, cmdStack);
}


//---------------------------------------------------------------------------
void LabConsole::Load(string file_path)
{
	string cmdStack_bkp = cmdStack;
	cmdStack += "\n" + LabUtils::FileToString(file_path);
	ExecuteStack(cmdStack_bkp);
}


//---------------------------------------------------------------------------
/** \brief Check for external keywords.
 */
int LabConsole::CheckKeywords(const char *a_szLine)
{
	string sLine(a_szLine);
	// Trim command
	sLine = LabUtils::RemoveChars(sLine, " \n\r\t\";");

	if ( sLine == _T("quit") )
	{
		return -1;
	}
	else if ( sLine == _T("cls") )
	{
		ClearScreen();
		return 1;
	}
	else if ( sLine == _T("reset") )
	{
		ResetCmdStack();
		return 1;
	}
	else if ( sLine == _T("listvar") )
	{
		// TODO
		return 1;
	}
	else if ( sLine.substr(0, 5).compare(_T("save2")) == 0 )
	{
		sLine = LabUtils::RemoveChars(sLine, "()");
		Save2(sLine.substr(5, sLine.length()));
		return 1;
	}
	else if ( sLine.substr(0, 4).compare(_T("save")) == 0 )
	{
		sLine = LabUtils::RemoveChars(sLine, "()");
		Save(sLine.substr(4, sLine.length()));
		return 1;
	}
	else if ( sLine.substr(0, 3).compare(_T("run")) == 0 )
	{
		sLine = LabUtils::RemoveChars(sLine, "()");
		string plug_name = sLine.substr(3, sLine.length());
		if (!plug_name.length()) Run(pluginName);
		else Run(plug_name);
		return 1;
	}
	else if ( sLine.substr(0, 3).compare(_T("del")) == 0 )
	{
		sLine = LabUtils::RemoveChars(sLine, "()");
		Delete(sLine.substr(3, sLine.length()));
		return 1;
	}
	else if ( sLine== _T("listplug") )
	{
		ListPlugins();
		return 1;
	}
	else if ( sLine.substr(0, 4).compare(_T("load")) == 0 )
	{
		sLine = LabUtils::RemoveChars(sLine, "()");
		Load(sLine.substr(4, sLine.length()));
		return 1;
	}

	return 0;
}


//---------------------------------------------------------------------------
//int LabConsole::Preparse(string s_line)
//{
//	string s_out = "";
//
//	// Split sub-expressions
//	vector<string> tokens = vector<string>();
//
//	LabUtils::Tokenize(s_line, tokens, ",", true);
//
//	for (unsigned int i=0; i< tokens.size(); i++)
//	{
//		// If we get a Vector declaration
//		string::size_type start_pos, end_pos;
//		start_pos = tokens[i].find("[");
//		end_pos = tokens[i].find("]");
//		if (start_pos != string::npos)
//		{
//			if (end_pos != string::npos)
//			{
//				if (end_pos - start_pos > 1)
//				{
//					istringstream buffer(tokens[i].substr(start_pos+1, end_pos-(start_pos+1)));
//					int size;
//					buffer >> size;
//					//AddVector(tokens[i].substr(0, start_pos), size, parser);
//				}
//				else
//				{
//					elp::console() << "No size specified for vector '" << tokens[i].substr(0, start_pos) << "'" << endl;
//					return -1;
//				}
//			}
//			else
//			{
//				elp::console() << "Bad token '[' found !" << endl;
//				return -1;
//			}
//		}
//		else
//		{
//			s_out += tokens[i] + ",";
//		}
//	}
//
//	if (s_out != "") s_out = s_out.substr(0, s_out.length()-1);
//	s_line = s_out;
//
//	return 1;
//}
//
//

/**
 * Tries to execute the current command stack
 * Recovery from "cmdStack_bkp" in case of failure
 */
void LabConsole::ExecuteStack(string cmdStack_bkp)
{
	LabPluginsManager *pluginManager = LabPluginsManager::GetInstance();

	std::vector<std::string> tokens;
	std::vector<std::string>::iterator it;
	LabUtils::Tokenize(cmdStack, tokens, ";", true);

	// Handle print operations
	string s_trim;
	string s_print;
	size_t pos, pos2;
	int len;
	cmdStack = "";
	for (it = tokens.begin() ; it < tokens.end(); it++ )
	{
		s_trim = (*it);
		if (LabUtils::RemoveChars(s_trim, " \t\n\r") != "")
		{
			pos = (*it).find("print");
			pos2 = (*it).find("print2");

			if (pos == std::string::npos)
				cmdStack += (*it) + ";";
			else
			{
				if (pos2 == std::string::npos) { len = 5; }
				else { len = 6; pos = pos2; }

#if  defined(_RUN_UNDER_ECLIPSE) || defined (_WIN32)
				s_print = "\ncout<<" + (*it).substr(pos + len) + "<<endl;";
				//cmdStack += "\ncout<<" + (*it).substr(pos + len) + "<<endl;";
#else
				s_print = "\ncout<<\"\\033[0;32m\"<<" + (*it).substr(pos + len) + "<<\"\\033[0m\"<<endl;";
				//cmdStack += "\ncout<<\"\\033[0;32m\"<<" + (*it).substr(pos + len) + "<<\"\\033[0m\"<<endl;";
#endif

				cmdStack += s_print;
				if (pos2 == std::string::npos) (*it) = "";
				else (*it) = s_print;
			}

		} else {
			//tokens.erase(it); /// Should never remove a token while iterating !!
			(*it) = "";
		}
	}

	pluginManager->CreatePlugin(pluginName, cmdStack);

	pluginManager->BuildPluginsLib();
	if (pluginManager->ready)
	{
		// Execute new feature
		pluginManager->ExecutePlugin(pluginName, false);

		// Clear of print operations
		cmdStack = "";
		for (it = tokens.begin() ; it < tokens.end(); it++ )
			if ((*it) != "") cmdStack += (*it) + ";";

	} else {
		// Resume last cmd stack state
		cmdStack = cmdStack_bkp;
		// Resume plugin file to the last compiling state
		pluginManager->CreatePlugin(pluginName, cmdStack);
	}
}

//---------------------------------------------------------------------------
/*
 * Compile current cmd stack & run
 */
void LabConsole::Calculate()
{

	try
	{

		// Create console dedicated plugin
		LabPluginsManager *pluginManager = LabPluginsManager::GetInstance();

		string cmdStack_bkp = cmdStack;

		for(;;)
		{

			cmdStack_bkp = cmdStack;

			// ELP PROMPT
			elp::console() << _T(LabUtils::ColorString("-ELP-$ > ", 33)) << flush;

			string sLine = "";
			char c;
			while (elp::console_in().good())
			{
				//std::getline(elp::console_in(), sLine);
				//elp::console_in() >> sLine;
				elp::console_in().get(c);
				if (c == '$') break;
				sLine += c;
			}

			//sLine = LabUtils::StringReplace(sLine, "\"", "\\\"");

			//elp::console() << sLine << endl;

			switch (CheckKeywords(sLine.c_str()))
			{
			case  0: break;
			case  1: continue;
			case -1: return;
			}

			string s_trim = sLine;
			LabUtils::RemoveChars(s_trim, " \t\n\r");

			////if (!sLine.length()) continue;
			if (!s_trim.length()) continue;

			cmdStack += sLine + "\n";

			//if (this->Preparse(sLine) == -1) continue;

			elp::console() << std::setprecision(12);

			// TODO : display a var or array if the line only consists in a var or array or array cell

			// Execute the command (i.e : recompile and execute the cmd stack)
			// Print exception message if any error
			ExecuteStack(cmdStack_bkp);

		} // while running

		pluginManager->DeletePlugin(pluginName);
		cmdStack = "";
	}
	catch(...)
	{
		throw;
	}

}


void LabConsole::Start() {

	Splash();
	Help();

	try
	{
		this->Calculate();
	}
	catch(std::exception & exc)
	{
		elp::console() << _T("Initialization error:  ") << exc.what() << endl;
		elp::console() << _T("aborting...\n");
	}

}

} /* namespace elp */
