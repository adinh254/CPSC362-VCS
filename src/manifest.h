#ifndef MANIFEST_H
#define MANIFEST_H

#include <string>
#include <filesystem>
#include <fstream>
#include <chrono>

namespace fs = std::filesystem;

fs::path getManifestPath( const fs::path& repo );
void createManifest(const fs::path& manifest_path);
void writeToManifest(const fs::path& manifest_path, const fs::path& file_path);
std::string getTimeStamp();

#endif // !MANIFEST_H

