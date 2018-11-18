#include "repo.h"
#include "artifact.h"
#include "manifest.h"

#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

// Create new repo directory.
// Recursively iterate through entire project tree and copy contents to destination repo.
// Case to check if path is a file.
void createRepo(const std::string& root, const std::string& dst) {
	createVersion(root, dst, 1);
}

void createVersion(const std::string& root, const std::string& dst, int version) {
	fs::path repo = dst;
	auto manifest_path = getManifestPath(repo, version);

	createManifest(manifest_path, root, dst);

	for (auto &content : fs::recursive_directory_iterator(root)) {
		auto repo_content = repo / fs::relative(content, root);

		fs::create_directory(repo_content);

		if (fs::is_regular_file(content.path())) {
			fs::path repo_content_file = repo_content / content.path().filename();
			fs::copy_file(content.path().string(), repo_content_file.string());
			
			std::fstream repo_content_file_contents(repo_content_file.string());

			auto id = getArtifactID(repo_content_file, repo_content_file_contents);

			repo_content_file_contents.close();

			fs::rename(repo_content_file.string(), id.string());

			writeToManifest(manifest_path, fs::relative(id, repo));
		}
	}
}

int getLatestVersion(const std::string &src) {
	int latest_version = 0;
	for (auto &content : fs::directory_iterator(src)) {
		std::string file_name = content.path().stem().string();
		if (file_name.find("manifest_") != std::string::npos) {
			size_t last_index = file_name.find_last_not_of("0123456789");
			std::string num = file_name.substr(last_index + 1);
			int version = stoi(num);
			if (latest_version < version) {
				latest_version = version;
			}
		}
	}
	return latest_version;
}

void checkoutUsingManifest(const std::string &src, const std::string &dst, const std::string &manifest, const std::string &label = "") {

	std::cout << "\ncheckoutUsingManifest" << std::endl;
	fs::path test = "test";
	test /= "test";
	std::string dir_symbol = test.string().substr(4,1);
	auto check_label = false;
	auto in_source_files = false;

	if (label != "") {
		check_label = true;
	}

	std::ifstream manifest_file(manifest);

	std::string line;
	while (std::getline(manifest_file, line)) {
		if (check_label) {
			if (line == "Label: " + label) {
				check_label = false;
			}
		}
		else {
			if (!in_source_files) {
				if (line == "Source Files:") {
					in_source_files = true;
				}
			}
			else {
				if (line.substr(0, 15) == "Time of Command:") {
					break;
				}
				else {
					fs::path src_path = src;
					fs::path dst_path = dst;
					src_path /= line;
					dst_path /= line.substr(0, line.find_last_of(dir_symbol));
					try {
						fs::create_directory(dst_path.parent_path());
						fs::copy_file(src_path.string(), dst_path.string(), fs::copy_options::recursive);
					}
					catch(fs::filesystem_error& e) {
						std::cout << "Could not copy " << line << " " << e.what() << '\n';
					}
				}
			}
		}
	}
	manifest_file.close();
}

// src argument is manifest path
// dst is target repo path
void checkout(const std::string& src, const std::string& dst, const std::string& manifest_info) {
	if( manifest_info.find( "manifest" ) != std::string::npos ) {
		// check if user specified manifest with extension.
		std::string manifest_name = manifest_info;
		std::string ext = ".txt";
		if( manifest_info.rfind( ".txt" ) == std::string::npos) {
			manifest_name += ext;
		}

		fs::path manifest_path = findManifestByName( src, manifest_name );
		if( manifest_path.empty() ) {
			std::cerr << "Label does not exist!" << '\n';
		}
		checkoutUsingManifest( src, dst, manifest_path.string() );
	}
	else {
		fs::path manifest_path = findManifestByLabel( src, manifest_info );
		if( manifest_path.empty() ) {
			std::cerr << "Label does not exist!" << '\n';
		}
		else {
			checkoutUsingManifest( src, dst, manifest_path.string(), manifest_info );
		}
	}

}

void checkin(const std::string& src, const std::string &dst) {
	auto version = getLatestVersion(dst);

	createVersion(src, dst, version);
}