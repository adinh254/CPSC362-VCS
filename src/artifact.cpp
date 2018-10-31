#include "artifact.h"

#include <array>
#include <vector>
#include <string>

namespace fs = std::filesystem;

// Array and vector to store weighted numbers and file content.
// For loop to calculate check sum.
std::string getArtifactID(const fs::path& path, std::fstream& my_file) {
	std::array <int, 5> weight = { 1, 3, 5, 7, 17 };
	std::vector<char> artifact;

	char ch;
	unsigned int file_size = 0;
	unsigned int sum = 0; // Unsigned prevents integer overflow.
	std::string art_id = "";

	// Get file content.
	while (my_file.get(ch)) {
		artifact.push_back(ch);
		file_size++;
	}

	// Calculate check sum.
	unsigned int weight_index = 0;
	for (auto& c : artifact) {
		sum += c * weight[weight_index];
		if (weight_index == 4)
			weight_index = 0;
		else
			weight_index++;
	}

	// https://en.cppreference.com/w/cpp/filesystem/path/parent_path
	// https://en.cppreference.com/w/cpp/filesystem/path/extension
	art_id = path.parent_path().string() + "/" + std::to_string(sum) + "-L" + std::to_string(file_size) + path.extension().string();

	// Close file to enable renaming.
	my_file.close();

	return art_id;
}

void renameArtifact(const fs::path& path) {
	std::fstream my_file;
	my_file.open(path);

	if (std::rename(path.string().c_str(), getArtifactID(path, my_file).c_str())) {
		std::perror("Error Renaming");
	}
}
