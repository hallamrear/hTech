#pragma once

#define PROJECT_DIRECTORY_PATH_COUNT 3

class Project;

class ProjectLoader
{
	static Project* mCurrentProject;
	
	inline static std::string ProjectDirectoryLayout[PROJECT_DIRECTORY_PATH_COUNT] =
	{
		"\\Assets\\",
		"\\Scenes\\",
		"\\Scripts\\"
	};

	static void CreateEngineProjectsFolder(std::string path);
	static void CreateEmptyProjectHierarchy(std::string projectName, std::string folderRoot);

public:
	static void CreateProject(std::string projectName);
	static void LoadProject(std::string projectName);
	static void UnloadProject(bool save = true);
	static void SaveProject();

	//see also PrototypeLibrary\Engine\AssetCore.cpp
	//and
	//https://www.appsloveworld.com/cplus/100/33/get-path-to-my-documents
};

class Project
{
	bool m_IsLoaded;

	

};