#ifndef MANIFEST_H
#define MANIFEST_H

#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <chrono>
#include <vector>

namespace fs = std::filesystem;

fs::path getManifestPath(const fs::path& repo, int version);
std::vector<std::string> getManifestsFromPath(const fs::path& repo);
int getMostRecentManifest(const fs::path& repo);
fs::path findManifestByLabel(const fs::path& repo, const std::string &label);
void createManifest(const fs::path& manifest_path, const std::string createRepoArg1, const std::string createRepoArg2);
void writeToManifest(const fs::path& manifest_path, const fs::path& file_path);
std::string getTimeStamp();

bool isLabelInManifest(const std::string &label);
void writeLabel(const std::string &label, const std::string &dst);
void addLabel(const std::string &label, const std::string &manifest_path);

#endif // !MANIFEST_H

