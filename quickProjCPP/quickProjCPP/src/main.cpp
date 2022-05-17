#include <iostream>
#include <filesystem>

#include "util.h"
#include "project.h"

int main() {
	ConsoleUtil::WriteTitle('=', "QuickProj");

	Project project;

	project.QueryName();
	project.QueryStorePath();
	project.StoreOnDisc();
	project.QueryGit();

	while (true);
}