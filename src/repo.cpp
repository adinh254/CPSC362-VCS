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

void checkout(const std::string& src, const std::string &dst) {
	fs::path potential_manifest = dst;
	if (fs::exists(potential_manifest)) {
		std::cout << "checkout from dst\n\n";
	}
	else {
		if (isLabelInManifest(dst)) {
			std::cout << "checkout from manifest.txt\n\n";
		}
	}
}

void checkin(const std::string& src, const std::string &dst) {
	auto version = getLatestVersion(dst);

	createVersion(src, dst, version);
}