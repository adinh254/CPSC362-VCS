#include <iostream>
#include <string>

#include "manifest.h"
#include "artifact.h"
#include "repo.h"

int main(int argc, char *argv[]) {
	if (argc < 4) {
		std::cout << "usage: " << argv[0] << " COMMAND SOURCE TARGET" << std::endl;
		return -1;
	}

	std::string cmd = argv[1];
	std::string arg_2 = argv[2]; // path to project folder
	std::string arg_3 = argv[3]; // path to empty repo folder
	if (cmd == "create") {
		createRepo(arg_2, arg_3);
	}

	return 0;
}
