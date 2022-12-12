#include "pch.h"
#include "ProjectLoader.h"
#include "Log.h"
#include <ShlObj_core.h>
#include <fstream>
#include "World.h"
#include "TextureCache.h"
#include <rapidjson.h>
#include <stringbuffer.h>
#include <prettywriter.h>
#include <reader.h>
#include <document.h>
#include "Console.h"
#include "Editor.h"

bool ProjectLoader::m_HasProjectLoaded = false;
std::string ProjectLoader::m_ProjectName = "No Project Loaded";

void ProjectLoader::LoadProject(std::filesystem::path sceneFilePath)
{
	if (m_HasProjectLoaded == true)
	{
		UnloadProject();
	}

	std::fstream stream;
	stream.open(sceneFilePath, std::ios::in | std::ios::end | std::ios::binary);

	if (stream.good())
	{
		stream.seekg(0, std::ios::end); int end = stream.tellg();
		stream.seekg(0, std::ios::beg); int start = stream.tellg();
		int size = end - start;

		char* buffer = new char[size + 1];
		stream.read(buffer, size);

		rapidjson::StringStream stream(buffer);

		Log::LogMessage(LogLevel::LOG_ERROR, buffer);

		//IMPLEMENT Loading Console variables from project.
		 		
		/// ---- This section was previously underneath m_HasProjectLoaded = true;
		m_ProjectName = sceneFilePath.filename().string();
		//Remove extension.
		m_ProjectName = m_ProjectName.substr(0, m_ProjectName.size() - sceneFilePath.extension().string().size()).c_str();

		std::filesystem::path projectPath = sceneFilePath;
		projectPath = projectPath.parent_path().string() + "\\Assets\\";

		//If the assets folder exists, try and load all the assets in it, to their caches.
		if (std::filesystem::exists(projectPath))
		{
			for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(projectPath))
			{
				TextureCache::GetTexture(dirEntry.path().filename().string());
			}
		}
		///----


		if (buffer != "")
		{
			rapidjson::Document loadedDoc;
			loadedDoc.Parse<rapidjson::kParseStopWhenDoneFlag>(buffer);
			World::Deserialize(loadedDoc);
		}

		m_HasProjectLoaded = true;

	}
	else
	{
		//IMPLEMENT FILE ERROR THROW;
		m_HasProjectLoaded = false;
		m_ProjectName = "No Project Loaded";
		throw;
	}

	stream.close();
}

void ProjectLoader::UnloadProject(bool save)
{
	if (m_HasProjectLoaded)
	{
		if (save)
		{
			SaveProject();
		}

#ifdef _DEBUG
		Editor::ClearSelected();
#endif
		TextureCache::UnloadAll();
		World::UnloadAll();

		m_HasProjectLoaded = false;
		m_ProjectName = "No Project Loaded";
	}
}

void ProjectLoader::SaveProject()
{
	if (m_HasProjectLoaded)
	{
		rapidjson::StringBuffer sb;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);

		//IMPLEMENT Saving Console variables from project.

		writer.StartObject();
		World::Serialize(writer);
		writer.EndObject();

		std::string fileName;
		GetEngineProjectsLocation(fileName);
		fileName += (m_ProjectName + "\\" + m_ProjectName + ".hProj");

		std::fstream fileStream;
		fileStream.open(fileName, std::ios::out | std::ios::trunc);

		if (fileStream.good())
		{
			fileStream.seekg(0, std::ios::beg);
			fileStream.write(sb.GetString(), sb.GetSize());
		}
		else
		{
			//IMPLEMENT file throw error.
			throw;
		}

		fileStream.close();
	}
}

void ProjectLoader::GetEngineProjectsLocation(std::string& path)
{
	//Gets the location of the documents folder, and creates the engines folders there.
	char buffer[512];
	SHGetFolderPathA(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, buffer);
	path = buffer;
	path += "\\hTech\\Projects\\";
}

const std::string ProjectLoader::GetLoadedProjectName()
{
	return m_ProjectName;
}

const bool ProjectLoader::HasProjectLoaded()
{
	return m_HasProjectLoaded;
}

void ProjectLoader::CreateProject(std::string projectName)
{
	UnloadProject();

	//Gets the location of the documents folder, and creates the engines folders there.
	std::string projectRootLocation;
	char buffer[512];
	SHGetFolderPathA(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, buffer);
	projectRootLocation = buffer;
	projectRootLocation += "\\hTech\\Projects\\";

	//Checks if the folder exists, if not, creates.
	if (std::filesystem::exists(projectRootLocation + projectName) == false)
	{
		Log::LogMessage(LogLevel::LOG_MESSAGE, "ProjectLoader -> Created engine project hierarchy root.");
		CreateEmptyProjectHierarchy(projectName, projectRootLocation);

		std::filesystem::path solutionPath = projectRootLocation + projectName + '\\' + std::string(projectName + ".sln");
		std::string command = "start devenv " + solutionPath.string();
		system(command.c_str());

		m_HasProjectLoaded = true;
		m_ProjectName = projectName;

		SaveProject();
	}
}

void ProjectLoader::CreateEmptyProjectHierarchy(const std::string& projectName, const std::string& folderRoot)
{
	std::filesystem::path projectFolderRoot = folderRoot + projectName;

	//If the project does not exist, create it
	if (std::filesystem::exists(folderRoot + projectName) == false)
	{
		CreateScriptSolution(folderRoot, projectName);

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

void ProjectLoader::ReplaceExampleReferencesInFile(const std::filesystem::path& filePath, const std::string& projectName)
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
						while (pos = line.find(wordToReplace), pos != std::string::npos)
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

void ProjectLoader::CreateScriptSolution(const std::filesystem::path& projectFolderRoot, const std::string& projectName)
{
	//Copies and unzips the source folder.
	std::filesystem::path sourceLocation = "Assets/HTECH_SCRIPT.zip";
	std::filesystem::path destinationLocation = projectFolderRoot;
	std::string command = "PowerShell -Command \"Expand-Archive -Path " + sourceLocation.string() + " -DestinationPath " + destinationLocation.string() + " -Force \"";
	system(command.c_str());
	//This section renames all the folders and directories to match the project name.
	std::vector<std::string> mFilePaths;
	std::vector<std::string> mAlteredFilePaths;
	std::vector<std::string> mDirectoryPaths;
	std::vector<std::string> mAlteredDirectoryPaths;

	//Gets all the paths for files and stuff and sorts them into files and directories.
	for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(projectFolderRoot))
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
}