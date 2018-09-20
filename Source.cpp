#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <stdio.h>



int main(int argc, char *argv[]) {
	if (argc < 4) {
		std::cout << "usage: " << argv[0] << " COMMAND SOURCE TARGET" << std::endl;
		return 0;
	}
	std::string cmd = argv[1];
	std::string source = argv[2]; // path to project folder
	std::string target = argv[3]; // empty repo folder


	//You can see the class output functionality by uncommenting

	/*artifact *a1 = new artifact("testArtifact1");
	artifact *a2 = new artifact("testArtifact2");
	artifact *a3 = new artifact("testArtifact3");
	std::vector<artifact *> artVec = { a1,a2,a3 };


	folder *f1 = new folder("folder1", {}, artVec);
	folder *f2A = new folder("folder2A");
	folder *f2BA = new folder("folder2BA");
	folder *f2BC = new folder("folder2BC");
	folder *f2B = new folder("folder2B", {f2BA, f2BC});
	folder *f2C = new folder("folder2C");
	folder *f3A = new folder("folder3A");
	folder *f2 = new folder("folder2", { f2A, f2B, f2C });
	folder *f3 = new folder("folder3", {f3A});
	folder *root = new folder("rootFolder", { f1,f2,f3 });
	std::cout << *root << std::endl;
	root->dispayTree();*/
	
	
	return 0;
}
