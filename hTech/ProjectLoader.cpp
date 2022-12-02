#include "pch.h"
#include "ProjectLoader.h"

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
}