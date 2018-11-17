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
		auto content_path = content.path();
		if (content_path.string().find("manifest_") != std::string::npos) {
			auto file_name = content_path.string();
			auto start = file_name.find("_");
			auto end = file_name.find(".");
			auto version = std::stoi(file_name.substr(start + 1, end - 1));

			if (latest_version < version) {
				latest_version = version;
			}
		}
	}
	return latest_version;
}

void checkoutUsingManifest(const std::string &src, const std::string &dst, const std::string &manifest, const std::string &label = "") {
	fs::path src_path = src;
	fs::path manifest_path = manifest;

	fs::create_directory(dst);

	// Copy everything from parent path of manifest
	fs::copy( manifest_path.parent_path(), dst, fs::copy_options::recursive );
	fs::path checkout_manifest = dst / manifest_path.filename();
	// Insert checkout commands to copied manifest
	std::ofstream temp( "temp.txt" );
	std::ifstream manifest_file( checkout_manifest );

	temp << "check-out Arguments: " << src << ' ' << dst;
	if (label != "") {
		temp << ' ' << label;
	}
	temp << '\n';
	temp << manifest_file.rdbuf();

	manifest_file.close();
	temp.close();

	fs::remove( checkout_manifest );
	fs::rename( "temp.txt", checkout_manifest);
}

// src argument is manifest path
// dst is target repo path
void checkout(const std::string& src, const std::string& dst) {
	auto version = getLatestVersion(src);
	fs::path potential_manifest = src;

	potential_manifest /= "manifest_" + std::to_string(version) + ".txt";

	if (fs::exists(potential_manifest)) {
		checkoutUsingManifest(src, dst, potential_manifest.string());
	}
}

// overloaded if user wants label as an argument
void checkout(const std::string& src, const std::string& dst, const std::string& label) {
	fs::path manifest_path = findManifestByLabel( src, label );
	if( fs::is_empty( manifest_path ) ) {
		std::cerr << "Label does not exist!" << '\n';
	}
	else {
		checkoutUsingManifest(src, dst, manifest_path.string(), label);
	}
}

void checkin(const std::string& src, const std::string &dst) {
	auto version = getLatestVersion(dst);

	createVersion(src, dst, version);
}