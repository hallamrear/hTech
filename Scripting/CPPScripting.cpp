// CPPScripting.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

#ifdef _WIN32
#include <Windows.h>
#include <libloaderapi.h>
#endif
#include "Script.h"

struct ScriptManager
{
	static Script* LoadDLL(std::string location)
	{
		typedef Script* (__stdcall *scriptPtr)();
		 
		//get raw script location
		std::string tempPath = "add any additional directories here." + location;
		tempPath = location;

		//Load libary into handle instance
		//loadA loads out DLL into memory for the progra,
		HINSTANCE hGetProcIDDLL = LoadLibraryA((tempPath.c_str()));

		//validate
		if (!hGetProcIDDLL)
		{
			std::cout << "Couldnot load library" << std::endl;
			return nullptr;
		}

		//GetPRocAddress returns the ptr to the function we are trying to find within the DLL instance.
		scriptPtr CreateScript = (scriptPtr)GetProcAddress(hGetProcIDDLL, "CreateScript");

		if (!CreateScript)
		{
			std::cout << "Could not find function name" << std::endl;
			return nullptr;
		}

		//resolve function address
		Script* function = CreateScript();
		return function;
	};
};


int main()
{
	Script* ptr;
	ptr = ScriptManager::LoadDLL("../Debug/ScriptSolutionTemplate.dll");

	if (ptr != nullptr)
	{
		while (true)
		{
			ptr->Update(0.016f);
		}
	}
	else
	{
		system("pause");
	}
}