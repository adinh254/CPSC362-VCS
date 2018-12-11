#ifndef ARTIFACT_H
#define ARTIFACT_H

#include <filesystem>
#include <fstream>

// Calculates content of artifact to get ID.
std::filesystem::path getArtifactID(const std::filesystem::path &path, std::fstream &myFile);

#endif