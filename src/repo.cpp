#include "repo.h"
#include "artifact.h"
#include "manifest.h"

#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

// Create new repo directory.
// Recursively iterate through entire project tree and copy contents to destination repo.
// Case to check if path is a file.
void createRepo(const::std::string& root, const std::string& dst) {
	// New repo directory
	fs::path root_last = root;
	fs::path repo = dst + "/";
	fs::path manifest_path = getManifestPath( repo );
	repo += root_last.filename();
	fs::create_directories(repo);
	createManifest( manifest_path );

	// Recursively iterate through project tree.
	// https://en.cppreference.com/w/cpp/filesystem/recursive_directory_iterator
	for (auto iter = fs::recursive_directory_iterator(root); iter != fs::recursive_directory_iterator(); ++iter) {
		std::error_code er;

		// Reassign destination path to current project tree path.
		// https://en.cppreference.com/w/cpp/filesystem/relative
		fs::path dst_content = repo.string() + "/";
		dst_content += fs::relative(iter->path(), root);

		fs::create_directory(dst_content, er);
		if (er) {
			std::cerr << "Error creating directory: " << dst_content << "\nFileSystem Error: " << er.message() << '\n';
		}

		// File Check Case
		if (fs::is_regular_file(iter->path())) {
			// Copy file and contents to current repo directory.
			copy(iter->path(), dst_content, er);
			if (er){
				std::cerr << "Error while copying: " << dst_content << "\nFileSystem Error: " << er.message() << '\n';
			}

			// Pointer to first file of current repo path.
			fs::directory_iterator firstEntry(dst_content);
			std::fstream my_file;
			my_file.open( firstEntry->path() );
			std::string art_id = getArtifactID( firstEntry->path(), my_file );
			my_file.close();
			fs::rename(firstEntry->path(), art_id);
			writeToManifest(manifest_path, art_id );
		}
	}
}
