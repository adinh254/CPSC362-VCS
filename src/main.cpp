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
		addLabel(arg2, arg1);
	}
	else if (cmd == "check-out") {
		std::string arg3 = argv[4];
		checkout( arg1, arg2, arg3 );
	}
	else if (cmd == "check-in") {
		checkin(arg1, arg2);
	}
	else if (cmd == "merge") {
		std::string arg3 = argv[4] ? argv[4] : "";
		merge(arg1, arg2, arg3);
	}
	else {
		std::cout << "Command not found." << std::endl;
	}

	return 0;
}
