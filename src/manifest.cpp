#include "manifest.h"

namespace fs = std::filesystem;

// Create new folder in top repo.
// Create symbolic link to latest repo manifest.
// Prints time and date in manifest.
fs::path getManifestPath( const fs::path& repo ) {
	fs::path root_path = repo;
	fs::path file_name = "manifest.txt";
	fs::path manifest_path = root_path / file_name;

	return manifest_path;
}
void createManifest(const fs::path& manifest_path) {
	std::ofstream out_file;
	out_file.open( manifest_path );
	out_file << "Time of Command: " << getTimeStamp() << '\n';
	out_file << "Source Files: \n";

	out_file.close();
}

// Write source file information to manifest. 
void writeToManifest(const fs::path& manifest_path, const fs::path& file_path) {
	std::ofstream out_file;
	out_file.open( manifest_path, std::ios::app );
	out_file << file_path.string() << '\n';
	out_file.close();
}

// https://stackoverflow.com/questions/34963738/c11-get-current-date-and-time-as-string
std::string getTimeStamp()
{
	std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
	std::time_t start_time = std::chrono::system_clock::to_time_t( now );
	char timedisplay[100];
	struct tm buf;
	errno_t err = localtime_s( &buf, &start_time );
	std::strftime( timedisplay, sizeof( timedisplay ), "%H:%M:%S %B %d, %Y", &buf );
	return timedisplay;
}
