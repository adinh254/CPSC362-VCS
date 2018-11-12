#include <iostream>
#include <string>

#include "manifest.h"
#include "artifact.h"
#include "repo.h"

int main(int argc, char *argv[]) {
	if (argc < 1) {
		std::cout << "You must specified a command." << std::endl;
	}

	std::string cmd = argv[1];

	if (cmd == "create") {
		createRepo(argv[2], argv[3]);
	}
	else if (cmd == "label") {
		addLabel(argv[2], argv[3]);
	}
	else if (cmd == "check-in") {
		std::cout << "Check in" << std::endl;
	}
	else if (cmd == "check-out") {
		std::cout << "Check out" << std::endl;
	}
	else {
		std::cout << "Command not found." << std::endl;
	}

	return 0;
}
