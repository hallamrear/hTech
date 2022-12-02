#pragma once
#include <filesystem>

#define PROJECT_DIRECTORY_PATH_COUNT 4

class Project;

class ProjectLoader
{
	static Project LoadedProject;
	
	inline static std::string ProjectDirectoryLayout[PROJECT_DIRECTORY_PATH_COUNT] =
	{
		"\\Assets\\",
		"\\Scenes\\",
		"\\Scripts\\",
		"\\Logs\\"
	};

	static void CreateEmptyProjectFile(const std::string& projectName);
	static void CreateScriptSolution(const std::filesystem::path& projectFolderRoot, const std::string& projectName);
	static void CreateEmptyProjectHierarchy(const std::string& projectName, const std::string& folderRoot);
	static void ReplaceExampleReferencesInFile(const std::filesystem::path& filePath, const std::string& projectName);

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
private:
	bool m_IsLoaded;
	std::filesystem::path m_AssetDirectory;
	std::filesystem::path m_ScenesDirectory;
	std::filesystem::path m_ScriptsDirectory;

public:
	bool GetIsLoaded() const;
	const std::filesystem::path& GetAssetDirectory() const;
	const std::filesystem::path& GetScenesDirectory() const;
	const std::filesystem::path& GetScriptsDirectory() const;
};