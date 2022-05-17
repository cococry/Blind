#pragma once

#include <string>

class Project
{
public:

	Project();

	Project(const std::string& name, const std::string& storePath);

	void QueryName();

	void QueryStorePath();

	bool StoreOnDisc();

	bool QueryGit();

	std::string Name, StorePath, FullPath;
	std::string GitRepositoryName, GitUserName;
private:
};