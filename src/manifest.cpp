#include "manifest.h"

namespace fs = std::filesystem;

//gets a the path of specific version manifest file inside a given repo.
fs::path getManifestPath(const fs::path &repo, int version)
{
	fs::path root_path = repo;
	fs::path file_name = "manifest_" + std::to_string(version) + ".txt";
	fs::path manifest_path = root_path / file_name;

	return manifest_path;
}

//returns a vector of paths of all the manifest files inside a given repo.
std::vector<std::string> getManifestsFromPath(const fs::path &repo)
{
	fs::path root_path = repo;
	std::vector<std::string> result;

	for (auto &p : fs::directory_iterator(root_path))
	{
		if (p.path().stem().string().find("manifest") != std::string::npos)
		{
			result.push_back(p.path().stem().string());
		}
	}

	return result;
}

//gets the the most recent manifest path for a given repo.
int getMostRecentManifest(const fs::path &repo)
{
	//get all the current manifests
	std::vector<std::string> manifests = getManifestsFromPath(repo);
	int largest = 0;
	for (auto it = manifests.begin(); it != manifests.end(); ++it)
	{
		std::string manifestName = *it;
		size_t last_index = manifestName.find_last_not_of("0123456789");
		std::string num = manifestName.substr(last_index + 1);
		int result = stoi(num);
		if (result > largest)
		{
			largest = result;
		}
	}

	return largest;
}

fs::path findManifestByLabel(const fs::path &repo, const std::string &label)
{
	//get all the current manifests
	std::vector<std::string> manifests = getManifestsFromPath(repo);
	// check each manifest for the label
	for (auto it = manifests.begin(); it != manifests.end(); ++it)
	{
		fs::path manifest_path = repo;

		manifest_path /= *it + ".txt";

		std::ifstream manifest(manifest_path.string());
		std::string line;
		// look at each line in the manifest for the label
		while (std::getline(manifest, line))
		{
			if (line == "Label: " + label)
			{
				manifest.close();
				return repo / (*it + ".txt");
			}
		}
		manifest.close();
	}
	// return "" if not found
	return "";
}

void createManifest(const fs::path &manifest_path, const std::string createRepoArg1 = "", std::string createRepoArg2 = "")
{
	std::ofstream out_file;
	out_file.open(manifest_path);
	if (createRepoArg1 != "" && createRepoArg1 != "")
	{
		out_file << "Create Repo Arguments: " << createRepoArg1 << " " << createRepoArg2 << '\n';
	}
	out_file << "Time of Command: " << getTimeStamp() << '\n';
	out_file << "Source Files:\n";

	out_file.close();
}

// Write source file information to manifest.
void writeToManifest(const fs::path &manifest_path, const fs::path &file_path)
{
	std::ofstream out_file;
	out_file.open(manifest_path, std::ios::app);
	out_file << file_path.string() << '\n';
	out_file.close();
}

// https://stackoverflow.com/questions/34963738/c11-get-current-date-and-time-as-string
std::string getTimeStamp()
{
	std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
	std::time_t start_time = std::chrono::system_clock::to_time_t(now);
	char timedisplay[100];
	struct tm buf;
	errno_t err = localtime_s(&buf, &start_time);
	std::strftime(timedisplay, sizeof(timedisplay), "%H:%M:%S %B %d, %Y", &buf);
	return timedisplay;
}

bool isLabelInManifest(const std::string &label)
{
	std::ifstream manifest("manifest.txt", std::ios::app);
	std::string line;
	while (std::getline(manifest, line))
	{
		if (line == "Label: " + label)
		{
			manifest.close();
			return true;
		}
	}
	manifest.close();
	return false;
}

void writeLabel(const std::string &dst, const std::string &label)
{
	std::ofstream temp("temp.txt");
	std::ifstream manifest(dst);

	temp << "Label: " + label + '\n';
	temp << manifest.rdbuf();

	manifest.close();
	temp.close();

	fs::remove(dst);
	fs::rename("temp.txt", dst);
}

void addLabel(const std::string &label, const std::string &dst)
{
	fs::path potential_manifest = dst;
	if (fs::exists(potential_manifest))
	{
		writeLabel(potential_manifest.string(), label);
	}
	else
	{
		auto manifest_path = findManifestByLabel(fs::current_path(), dst);

		if (manifest_path.empty())
		{
			std::cerr << "Label does not exist!" << '\n';
		}
		else
		{
			writeLabel(manifest_path.string(), label);
		}
	}
}

void backtrackManifest(const std::string manifest, std::vector<std::string> &list)
{
	fs::path manifest_path = manifest;

	std::ifstream manifest_file(manifest);
	std::string line;
	while (std::getline(manifest_file, line))
	{
		if (line.find("Check Out") == 0)
		{
			size_t mid_point = line.find(':', 23);

			std::string past_source = line.substr(21, mid_point - 22);

			if (manifest_path.parent_path().string() == past_source)
			{
				size_t last_index = manifest.find_last_not_of("0123456789");
				std::string num = manifest.substr(last_index + 1);
				int version = stoi(num);
				std::string prev_manifest_file = "manifest_" + std::to_string(version - 1) + ".txt";

				fs::path prev_manifest = manifest_path.parent_path() / prev_manifest_file;

				if (fs::exists(prev_manifest))
				{
					list.push_back(prev_manifest.string());

					backtrackManifest(prev_manifest.string(), list);
				}
			}
			else
			{
				fs::path prev_manifest = past_source / manifest_path.filename();

				if (fs::exists(prev_manifest))
				{
					list.push_back(prev_manifest.string());

					backtrackManifest(prev_manifest.string(), list);
				}
			}

			break;
		}
		else if (line.find("Check In") == 0)
		{
			size_t mid_point = line.find_last_of(":");

			std::string past_source = line.substr(mid_point + 2);

			if (manifest_path.parent_path().string() == past_source)
			{
				size_t last_index = manifest.find_last_of("_");
				std::string num = manifest.substr(last_index + 1);
				int version = stoi(num);
				std::string prev_manifest_file = "manifest_" + std::to_string(version - 1) + ".txt";

				fs::path prev_manifest = manifest_path.parent_path() / prev_manifest_file;

				if (fs::exists(prev_manifest))
				{
					list.push_back(prev_manifest.string());

					backtrackManifest(prev_manifest.string(), list);
				}
			}
			else
			{
				fs::path prev_manifest = past_source / manifest_path.filename();

				if (fs::exists(prev_manifest))
				{
					list.push_back(prev_manifest.string());

					backtrackManifest(prev_manifest.string(), list);
				}
			}

			break;
		}
	}
	manifest_file.close();
}

std::string getMostRecentCommonAncestor(const std::string manifestPath1, const std::string manifestPath2)
{
	std::vector<std::string> manifest1_manifest_list;
	std::vector<std::string> manifest2_manifest_list;

	manifest1_manifest_list.push_back(manifestPath1);
	manifest2_manifest_list.push_back(manifestPath2);

	backtrackManifest(manifestPath1, manifest1_manifest_list);
	backtrackManifest(manifestPath2, manifest2_manifest_list);

	std::cout << "Manifest 1:\n";

	for (auto &manifest : manifest1_manifest_list)
	{
		std::cout << manifest << '\n';
	}

	std::cout << "\nManifest 2:\n";

	for (auto &manifest : manifest2_manifest_list)
	{
		std::cout << manifest << '\n';
	}

	// Reverse iterate through both vectors
	typedef std::vector<std::string>::reverse_iterator rIter;
	for( rIter it1 = manifest1_manifest_list.rbegin(), it2 = manifest2_manifest_list.rbegin(), 
		 end1 = manifest1_manifest_list.rend(), end2 = manifest2_manifest_list.rend(); 
		 it1 != end1 || it2 != end2; ++it1, ++it2 )
	{
		std::string common_ancestor;
		if( it1 == end1 || it2 == end2)
		{
			// Check if one iterator has reached end.
			if( manifest1_manifest_list.size() > manifest2_manifest_list.size() )
			{
				common_ancestor = *( it1 - 1 );
			}
			else
			{
				common_ancestor = *( it2 - 1 );
			}
			return common_ancestor;
		}
		else if( *it1 != *it2 )
		{
			common_ancestor = *( it1 - 1 );
			return common_ancestor;
		}
	}
	return "Common ancestor not found!";
}