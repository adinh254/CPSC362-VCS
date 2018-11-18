#include "manifest.h"

namespace fs = std::filesystem;

// Create new folder in top repo.
// Create symbolic link to latest repo manifest.
// Prints time and date in manifest.
fs::path getManifestPath(const fs::path& repo, int version) {
	fs::path root_path = repo;
	fs::path file_name = "manifest_" + std::to_string(version) + ".txt";
	fs::path manifest_path = root_path / file_name;

	return manifest_path;
}

std::vector<std::string> getManifestsFromPath(const fs::path& repo) {
	fs::path root_path = repo;
	std::vector<std::string> result;

	for (auto & p : fs::directory_iterator(root_path)) {
		if (p.path().stem().string().find("manifest") != std::string::npos) {
			result.push_back(p.path().stem().string());
		}
	}

	return result;
}

int getMostRecentManifest(const fs::path& repo) {
	//get all the current manifests
	std::vector<std::string> manifests = getManifestsFromPath(repo);
	int largest = 0;
	for (auto it = manifests.begin(); it != manifests.end(); ++it) {
		std::string manifestName = *it;
		size_t last_index = manifestName.find_last_not_of("0123456789");
		std::string num = manifestName.substr(last_index + 1);
		int result = stoi(num);
		if (result > largest) {
			largest = result;
		}
	}

	return largest;
}

fs::path findManifestByName( const fs::path& repo, const std::string& manifest_name ) {
	std::vector<std::string> manifests = getManifestsFromPath( repo );

	for( auto&& file_name : manifests ) {
		file_name += ".txt";
		if( file_name == manifest_name ) {
			return repo / ( file_name );
		}
	}
	return "";
}

fs::path findManifestByLabel(const fs::path& repo, const std::string &label) {
	//get all the current manifests
	std::vector<std::string> manifests = getManifestsFromPath(repo);
	// check each manifest for the label
	for (auto it = manifests.begin(); it != manifests.end(); ++it) {
		fs::path manifest_path = repo;
		
		manifest_path /=  *it + ".txt";

		std::ifstream manifest(manifest_path.string());
		std::string line;
		// look at each line in the manifest for the label
		while (std::getline(manifest, line)) {
			if (line == "Label: " + label) {
				manifest.close();
				return repo / (*it + ".txt");
			}
		}
		manifest.close();
	}
	// return "" if not found
	return "";
}

void createManifest(const fs::path& manifest_path, const std::string createRepoArg1 = "", std::string createRepoArg2 = "") {
	std::ofstream out_file;
	out_file.open( manifest_path );
	if (createRepoArg1 != "" && createRepoArg1 != "") {
		out_file << "Create Repo Arguments: " << createRepoArg1 << " " << createRepoArg2 << '\n';
	}
	out_file << "Time of Command: " << getTimeStamp() << '\n';
	out_file << "Source Files:\n";

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

bool isLabelInManifest(const std::string &label) {
	std::ifstream manifest("manifest.txt", std::ios::app);
	std::string line;
	while (std::getline(manifest, line)) {
		if (line == "Label: " + label) {
			manifest.close();
			return true;
		}
	}
	manifest.close();
	return false;
}

void writeLabel(const std::string &dst, const std::string &label) {
	std::ofstream temp("temp.txt");
	std::ifstream manifest(dst);

	temp << "Label: " + label + '\n';
	temp << manifest.rdbuf();

	manifest.close();
	temp.close();

	fs::remove(dst);
	fs::rename("temp.txt", dst);
}

void addLabel(const std::string &label, const std::string &dst) {
	fs::path potential_manifest = dst;
	if (fs::exists(potential_manifest)) {
		writeLabel(potential_manifest.string(), label);
	}
	else {
		auto manifest_path = findManifestByLabel( fs::current_path(), dst );

		if( manifest_path.empty() ) {
			std::cerr << "Label does not exist!" << '\n';
		}
		else {
			writeLabel(manifest_path.string(), label);
		}
	}
}