#include "pch.h"
#include "ProjectLoader.h"
#include "nlohmann/json.hpp"
#include "Log.h"
#include <ShlObj_core.h>
using json = nlohmann::json;

void ProjectLoader::LoadProject(std::string projectName)
{
	//IMPLEMENT Load Project
}

void ProjectLoader::UnloadProject(bool save)
{
	if (save)
	{
		SaveProject();
	}

	//IMPLEMENT Unload Project
}

void ProjectLoader::SaveProject()
{
	//IMPLEMENT Save Project
}

void ProjectLoader::CreateProject(std::string projectName)
{
	//IMPLEMENT Create Project

	//Gets the location of the documents folder, and creates the engines folders there.
	std::string projectRootLocation;
	char buffer[512];
	SHGetFolderPathA(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, buffer);
	projectRootLocation = buffer;
	projectRootLocation += "\\hTech\\Projects\\";

	//Checks if the folder exists, if not, creates.
	CreateEngineProjectsFolder(projectRootLocation);

	CreateEmptyProjectHierarchy(projectName, projectRootLocation);
}

void ProjectLoader::CreateEmptyProjectHierarchy(std::string projectName, std::string folderRoot)
{
	std::filesystem::path projectFolderRoot = folderRoot + projectName;

	//If the project does not exist, create it
	if (std::filesystem::exists(projectFolderRoot) == false)
	{
		std::filesystem::create_directories(projectFolderRoot);

		for (int i = 0; i < PROJECT_DIRECTORY_PATH_COUNT; i++)
		{
			std::filesystem::create_directories(projectFolderRoot.string().append(ProjectDirectoryLayout[i]));
		}
	}
	else
	{
		Log::LogMessage(LogLevel::LOG_ERROR, "ProjectLoader -> Trying to create a project with a name that already exists.");
	}
}

void ProjectLoader::CreateEngineProjectsFolder(std::string path)
{
	if (std::filesystem::exists(path) == false)
	{
		std::filesystem::create_directories(path);
		Log::LogMessage(LogLevel::LOG_MESSAGE, "ProjectLoader -> Created engine project hierarchy root.");
	}
}