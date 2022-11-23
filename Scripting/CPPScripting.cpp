// CPPScripting.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <string>

#ifdef _WIN32
#include <Windows.h>
#include <libloaderapi.h>
#include <filesystem>
#endif
#include "Script.h"

#include <windows.h>

typedef void(*Test)();

namespace DLLFunctions
{
	Test mTest;
}

// Create a string with last error message
std::string GetLastErrorStdStr()
{
	DWORD error = GetLastError();
	if (error)
	{
		LPVOID lpMsgBuf;
		DWORD bufLen = FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			error,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf,
			0, NULL);
		if (bufLen)
		{
			LPCSTR lpMsgStr = (LPCSTR)lpMsgBuf;
			std::string result(lpMsgStr, lpMsgStr + (bufLen * 3));

			LocalFree(lpMsgBuf);

			return result;
		}
	}
	return std::string();
}

void ReplaceExampleReferencesInFile(std::filesystem::path filePath, std::string projectName)
{
	std::ifstream stream(filePath.c_str());
	std::filesystem::path tempPath = filePath;
	tempPath.replace_extension("_2" + filePath.extension().string());

	if (stream)
	{
		if (stream.good())
		{
			std::string wordToReplace = "HTECH_SCRIPT";
			size_t len = wordToReplace.length();
			std::ofstream tempFile(tempPath.string());

			if (tempFile)
			{
				if (tempFile.good())
				{
					std::string line;
					while (std::getline(stream, line))
					{
						size_t pos = 1000000000000;
						while(pos = line.find(wordToReplace), pos != std::string::npos)
						{
							line.replace(pos, len, projectName);
						}					

						line += '\n';
						tempFile << line;
					}

				}
			}

			tempFile.close();
		}
	}

	stream.close();

	try
	{
		std::filesystem::rename(tempPath, filePath);
	}
	catch (std::filesystem::filesystem_error const& error)  // catch the errors(if any)!
	{
		std::cout << error.code() << "\n" << error.what() << "\n";
	}
}

void CreateProjectFolder(std::string projectName)
{
	//Check if projectName folder already exists
	bool exists = false;
	const std::filesystem::path mainPath = std::filesystem::current_path() / "Project";
	std::string path = "";
	for (auto& itr : std::filesystem::directory_iterator(mainPath))
	{
		if (itr.is_directory())
		{
			path = itr.path().string();
			if (itr.path().string().find(projectName) != path.npos)
			{
				std::cout << path;
				exists = true;
				break;
			}

			std::cout << " exists: " << exists << std::endl;
		}
	}

	if (exists == false)
	{
		//Copies and unzips the source folder.
		std::filesystem::path sourceLocation = "HTECH_SCRIPT.zip";
		std::filesystem::path destinationLocation = std::filesystem::current_path() / "Project\\";
		std::string command = "PowerShell -Command \"Expand-Archive -Path " + sourceLocation.string() + " -DestinationPath " + destinationLocation.string() + " -Force \"";
		system(command.c_str());

		//This section renames all the folders and directories to match the project name.
		std::vector<std::string> mFilePaths;
		std::vector<std::string> mAlteredFilePaths;
		std::vector<std::string> mDirectoryPaths;
		std::vector<std::string> mAlteredDirectoryPaths;

		//Gets all the paths for files and stuff and sorts them into files and directories.
		for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(mainPath))
		{
			if (dirEntry.path().string().find("HTECH_SCRIPT") != dirEntry.path().string().npos)
			{
				if (std::filesystem::is_directory(dirEntry))
				{
					mDirectoryPaths.push_back(dirEntry.path().string());
				}
				else if (std::filesystem::is_regular_file(dirEntry))
				{
					mFilePaths.push_back(dirEntry.path().string());
				}
			}
		}

		mAlteredFilePaths = mFilePaths;
		mAlteredDirectoryPaths = mDirectoryPaths;

		//Rename all files first to avoid making all directory names redundant.
		for (size_t i = 0; i < mFilePaths.size(); i++)
		{
			std::string toReplace = "HTECH_SCRIPT";
			mAlteredFilePaths[i].replace(mAlteredFilePaths[i].rfind(toReplace), toReplace.length(), projectName);

			try
			{
				std::filesystem::path from = mFilePaths[i];
				std::filesystem::path to = mAlteredFilePaths[i];
				std::filesystem::rename(from, to);

				ReplaceExampleReferencesInFile(mAlteredFilePaths[i], projectName);
			}
			catch (std::filesystem::filesystem_error const& error)  // catch the errors(if any)!
			{
				std::cout << error.code() << "\n" << error.what() << "\n";
			}
		}

		//Renames all the directories in reverse order, to avoid making outer folders unreachable with these paths
		//
		//	A//B     -> E//B
		//  A//B//C  -> A// NO LONGER EXISTS AS IT IS NOW E//B//C
		//
		//
		for (int x = (int)mDirectoryPaths.size() - 1; x >= 0; x--)
		{
			std::string toReplace = "HTECH_SCRIPT";
			mAlteredDirectoryPaths[x].replace(mAlteredDirectoryPaths[x].rfind(toReplace), toReplace.length(), projectName);

			try
			{
				std::filesystem::path from = mDirectoryPaths[x];
				std::filesystem::path to = mAlteredDirectoryPaths[x];
				std::filesystem::rename(from, to);
			}
			catch (std::filesystem::filesystem_error const& error)  // catch the errors(if any)!
			{
				std::cout << error.code() << "\n" << error.what() << "\n";
			}
		}

		std::filesystem::path solutionPath = mainPath / projectName / std::string(projectName + ".sln");
		command = "start devenv " + solutionPath.string();
		system(command.c_str());
	}
}

struct ScriptManager
{
	static Script* LoadDLL(std::string location, std::string fn)
	{
		typedef Script* (__cdecl* scriptPtr)();

		//get raw script location
		std::string tempPath = "add any additional directories here." + location;
		tempPath = location;

		//Load libary into handle instance
		//loadA loads out DLL into memory for the program
		HINSTANCE hGetProcIDDLL = LoadLibraryEx(tempPath.c_str(), NULL, 0);

		//validate
		if (!hGetProcIDDLL)
		{
			std::cout << "Could not load library" << std::endl;
			std::cout << GetLastErrorStdStr() << std::endl;
			return nullptr;
		}

		//GetPRocAddress returns the ptr to the function we are trying to find within the DLL instance.
		std::string functionName = fn;
		scriptPtr DLLFunction = (scriptPtr)GetProcAddress(hGetProcIDDLL, functionName.c_str());

 		if (!DLLFunction)
		{
			std::cout << "Could not find function name" << std::endl;
			std::cout << GetLastErrorStdStr() << std::endl;
			return nullptr;
		}

		//resolve function address
		Script* function = DLLFunction();
		return function;
	};
};

int main()
{
	std::string projectName = "bUILD";
	//CreateProjectFolder(projectName);

	std::string location = "Project//test//Debug//test.dll";	
	std::string fn = "?Create@TestClassOne@@SAPAVScript@@XZ";
	auto ptrOne = ScriptManager::LoadDLL(location, fn);
	fn = "?Create@TestClassTwo@@SAPAVScript@@XZ";
	auto ptrTwo = ScriptManager::LoadDLL(location, fn);

	if (ptrOne != nullptr && ptrTwo != nullptr)
	{
		while (true)
		{
			ptrOne->Update(0.016f);
			ptrTwo->Update(0.016f);
		}
	}
	else
	{
		system("pause");
	}
}