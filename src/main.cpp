#include <iostream>
#include <string>

#include "manifest.h"
#include "artifact.h"
#include "repo.h"

int main(int argc, char *argv[]) {
	if (argc < 4) {
		std::cout << "You must specified a command and at least two arguments." << std::endl;
	}

	std::string cmd = argv[1];
	std::string arg1 = argv[2];
	std::string arg2 = argv[3];

	if (cmd == "create") {
		createRepo(arg1, arg2);
	}
	else if (cmd == "label") {
		addLabel(arg1, arg2);
	}
	else if (cmd == "check-out") {
		checkout(arg1, arg2);
	}
	else if (cmd == "check-in") {
		std::cout << "Check in" << std::endl;
	}
	else {
		std::cout << "Command not found." << std::endl;
	}

	return 0;
}
