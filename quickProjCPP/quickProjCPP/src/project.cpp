#include "project.h"

#include <iostream>
#include <filesystem>
#include <fstream>

Project::Project()
	: Name(""), StorePath(""), FullPath("")
{
}

Project::Project(const std::string& name, const std::string& storePath)
	: Name(name), StorePath(storePath), FullPath(StorePath + "\\" + Name)
{
}

void Project::QueryName()
{
	std::cout << "Name of the project: ";
	std::cin >> Name;
}

void Project::QueryStorePath()
{
	
	std::cout << "Location to store the project: ";
	std::cin >> StorePath;
}

bool Project::StoreOnDisc()
{
	if (Name == "" || StorePath == "")
	{
		std::cout << "Tried to store project that was not configured correclty on disc.\n";
		return false;
	}
	FullPath = StorePath + "\\" + Name;
	if (std::filesystem::create_directory(FullPath)) 
	{
		std::cout << "Successfully created project at '" << FullPath << "'.\n";
		return true;
	}
	else
	{
		std::cout << "Failed to create project at '" << FullPath << "'.\n";

		if (std::filesystem::exists(FullPath)) 
		{
			std::cout << "Folder at location '" << FullPath << "' already exists.\n";
		}
		return false;
	}
	return false;
}

bool Project::QueryGit()
{
	std::string userAnsw;
	std::cout << "Link the project wit a git repository (Y/n): ";
	std::cin >> userAnsw;

	while (userAnsw != "y")
	{
		if (userAnsw == "Y")
			break;
		if (userAnsw == "n")
			break;
		if (userAnsw == "N")
			break;

		std::cout << "Y/n: ";
		std::cin >> userAnsw;
	}
	
	if (userAnsw == "Y" || userAnsw == "y")
	{
		std::cout << "Git user name: ";
		std::cin >> GitUserName;

		std::cout << "Name of the git repository: ";
		std::cin >> GitRepositoryName;

		std::string cdCmd = "cd /d " + FullPath;
		const char* cdCmdCSTR = cdCmd.c_str();
		std::cout << cdCmdCSTR << "\n";
		system(cdCmdCSTR);

		std::string gitInitCmd = "git init " + FullPath;
		const char* gitInitCmdCSTR = gitInitCmd.c_str();

		system(gitInitCmdCSTR);
		system("git add .");
		system("git commit -m \"Initial Commit\"");
		system("git branch -M master");

		std::string gitUrl = "https://github.com/" + GitUserName + "/" + GitRepositoryName;
		const char* gitUrlCSTR = gitUrl.c_str();
		std::string gitAddCmd = "git remote add origin " + gitUrl;
		const char* gitAddCmdCSTR = gitAddCmd.c_str();

		system(gitAddCmdCSTR);
		system("git push -u origin master");
		return true;
	}
	else 
	{
		std::cout << "Project will not link with git.";
	}
	return false;
}
