#pragma once
#include <filesystem>

#define PROJECT_DIRECTORY_PATH_COUNT 4

class Project;

class ProjectLoader
{	
	static std::string m_ProjectName;
	static bool	m_HasProjectLoaded;

	inline static std::string ProjectDirectoryLayout[PROJECT_DIRECTORY_PATH_COUNT] =
	{
		"\\Assets\\",
		"\\Scenes\\",
		"\\Scripts\\",
		"\\Logs\\"
	};

	static void CreateScriptSolution(const std::filesystem::path& projectFolderRoot, const std::string& projectName);
	static void CreateEmptyProjectHierarchy(const std::string& projectName, const std::string& folderRoot);
	static void ReplaceExampleReferencesInFile(const std::filesystem::path& filePath, const std::string& projectName);

public:
	/// <summary>
	/// Gets the name of the project currently loaded.
	/// </summary>
	/// <param name="name">Returns the name of the project.</param>
	static const std::string GetLoadedProjectName();
	static const bool HasProjectLoaded();
	static void CreateProject(std::string projectName);
	static void LoadProject(std::filesystem::path sceneFilePath);
	static void UnloadProject(bool save = true);
	static void SaveProject();
	static void GetEngineProjectsLocation(std::string& path);
};