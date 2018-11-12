#ifndef ARTIFACT_H
#define ARTIFACT_H

#include <filesystem>
#include <fstream>

// Calculates content of artifact to get ID.
std::string getArtifactID( const std::filesystem::path& path, std::fstream& myFile );
// Renames file name to artifact ID.
//void renameArtifact( const std::filesystem::path& path );

#endif