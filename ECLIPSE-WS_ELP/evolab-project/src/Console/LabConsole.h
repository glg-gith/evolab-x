/*
 * LabConsole.h
 *
 *  Created on: 4 juil. 2012
 *      Author: golgauth
 */

#ifndef LABCONSOLE_H_
#define LABCONSOLE_H_

//#include "muParser.h"
#include "Elp.h"
#include <Singleton.h>

#include <iostream>

//using namespace mu;
using namespace elp;

namespace elp {

#if defined(_UNICODE)

  //------------------------------------------------------------------------------
  /** \brief Encapsulate wcout. */
  inline std::wostream& console()
  {
    return std::wcout;
  }

  /** \brief Encapsulate cin. */
  inline std::wistream& console_in()
  {
    return std::wcin;
  }

#else

  /** \brief Encapsulate cout.

    Used for supporting UNICODE more easily.
  */
  inline std::ostream& console()
  {
    return std::cout;
  }

  /** \brief Encapsulate cin.

    Used for supporting UNICODE more easily.
  */
  inline std::istream& console_in()
  {
    return std::cin;
  }

#endif


class LabConsole : public Singleton<LabConsole> {
	friend class Singleton<LabConsole>;
private:
	LabConsole();
	~LabConsole();

	void Calculate();
	int Preparse(string s_line);
	void ExecuteStack(string cmdStack_bkp);
	void Splash();
	void Help();
	void ClearScreen();
	void ResetCmdStack();
	void ListPlugins();
	void Save(string plugin_name);
	void Run(string plugin_name);
	void Delete(string plugin_name);
	void Save2(string file_path);
	void Load(string file_path);
	int CheckKeywords(const char *a_szLine);

public:
	void Start();

private:
	string cmdStack;
	string pluginName;
};

} /* namespace elp */
#endif /* LABCONSOLE_H_ */
