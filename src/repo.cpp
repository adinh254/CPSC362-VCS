#include "repo.h"
#include "artifact.h"
#include "manifest.h"

#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

// Create new repo in a path, dst, from a given path root
void createRepo(const std::string &root, const std::string &dst)
{
	createVersion(root, dst, 1);
}

// creates a new version of a project. New version meaning a newly generated manifest.
void createVersion(const std::string &root, const std::string &dst, int version, bool checkin)
{
	fs::path repo = dst;
	auto manifest_path = getManifestPath(repo, version);

	createManifest(manifest_path, root, dst);

	if (checkin)
	{
		std::ofstream temp("temp.txt");
		std::ifstream manifest(manifest_path);

		temp << "Check In Arguments: " << root << " : " << dst << '\n';
		temp << manifest.rdbuf();

		manifest.close();
		temp.close();

		fs::remove(manifest_path);
		fs::rename("temp.txt", manifest_path);
	}
	for (auto &content : fs::recursive_directory_iterator(root))
	{
		auto repo_content = repo / fs::relative(content, root);

		fs::create_directory(repo_content);

		//skip is the manifest file.

		if (fs::is_regular_file(content.path()) && content.path().filename().string().find("manifest") == std::string::npos)
		{
			fs::path repo_content_file = repo_content / content.path().filename();
			try
			{
				fs::copy_file(content.path().string(), repo_content_file.string());
			}
			catch (fs::filesystem_error &e)
			{
				std::cout << "Could not copy file to repo when making new version. " << e.what() << '\n';
			}

			std::fstream repo_content_file_contents(repo_content_file.string());

			auto id = getArtifactID(repo_content_file, repo_content_file_contents);

			repo_content_file_contents.close();

			fs::rename(repo_content_file.string(), id.string());

			writeToManifest(manifest_path, fs::relative(id, repo));
		}
	}
}

//get the latest version (manifest) number of a repo
int getLatestVersion(const std::string &src)
{
	int latest_version = 0;
	for (auto &content : fs::directory_iterator(src))
	{
		std::string file_name = content.path().stem().string();
		if (file_name.find("manifest_") != std::string::npos)
		{
			size_t last_index = file_name.find_last_not_of("0123456789");
			std::string num = file_name.substr(last_index + 1);
			int version = stoi(num);
			if (latest_version < version)
			{
				latest_version = version;
			}
		}
	}
	return latest_version;
}

// adds the checkout arguments to a manifest.
void addCheckoutToManifest(const std::string &manifestPath, const std::string &arg1, const std::string &arg2, const std::string &arg3)
{
	std::ofstream temp("temp.txt");
	std::ifstream manifest(manifestPath);

	//temp << "Check Out Arguments: " << arg1 << " " << arg2 << " " << arg3 << '\n';
	temp << "Check Out Arguments: " << arg1 << " : " << arg2 << '\n';
	temp << manifest.rdbuf();

	manifest.close();
	temp.close();

	fs::remove(manifestPath);
	fs::rename("temp.txt", manifestPath);
}

// src argument is manifest path
// dst is target repo path
void checkout(const std::string &src, const std::string &dst, const std::string &manifest_info)
{
	if (manifest_info.find("manifest") != std::string::npos)
	{
		// check if user specified manifest with extension.
		std::string manifest_name = fs::path(manifest_info).filename().string();

		std::string ext = ".txt";
		if (manifest_info.rfind(ext) == std::string::npos)
		{
			manifest_name += ext;
		}
		fs::path srcPath = src;
		fs::path manifest_path;
		std::vector<std::string> manifests = getManifestsFromPath(srcPath);
		for (auto &&file_name : manifests)
		{
			file_name += ".txt";
			if (file_name == manifest_name)
			{
				manifest_path = srcPath / file_name;
				break;
			}
		}

		//std::cout << "manifestPath: " << manifest_path;
		if (manifest_path.empty())
		{
			std::cerr << "Manifest file does not exist!" << '\n';
		}
		else
		{
			continueCheckout(src, dst, manifest_path.string());
		}
	}
	else
	{
		fs::path manifest_path = findManifestByLabel(src, manifest_info);
		if (manifest_path.empty())
		{
			std::cerr << "Label does not exist!" << '\n';
		}
		else
		{
			continueCheckout(src, dst, manifest_path.string(), manifest_info);
		}
	}
}

void continueCheckout(const std::string &src, const std::string &dst, const std::string &manifest, const std::string &label)
{
	fs::path test = "test";
	test /= "test";
	std::string dir_symbol = test.string().substr(4, 1);
	auto check_label = false;
	auto in_source_files = false;

	if (label != "")
	{
		check_label = true;
	}

	fs::path new_manifest_path = dst;

	new_manifest_path /= fs::path(manifest).filename();

	try
	{
		fs::copy_file(manifest, new_manifest_path.string(), fs::copy_options::overwrite_existing);
	}
	catch (fs::filesystem_error &e)
	{
		std::cout << "Could not copy manifest to destination. " << e.what() << '\n';
	}

	std::string arg3 = check_label ? label : manifest;
	addCheckoutToManifest(new_manifest_path.string(), src, dst, arg3);

	std::ifstream manifest_file(manifest);

	std::string line;
	while (std::getline(manifest_file, line))
	{
		if (check_label)
		{
			if (line == "Label: " + label)
			{
				check_label = false;
			}
		}
		else
		{
			if (!in_source_files)
			{
				if (line == "Source Files:")
				{
					in_source_files = true;
				}
			}
			else
			{
				//skip copying timestamp and previous checkout args
				if (line.substr(0, 15) == "Time of Command:" || line.substr(0, 19) == "Check Out Arguments:")
				{
					break;
				}
				else
				{
					fs::path src_path = src;
					fs::path dst_path = dst;
					src_path /= line;
					dst_path /= line.substr(0, line.find_last_of(dir_symbol));
					try
					{
						fs::create_directories(dst_path.parent_path());
						fs::copy_file(src_path.string(), dst_path.string(), fs::copy_options::overwrite_existing);
					}
					catch (fs::filesystem_error &e)
					{
						std::cout << "Could not copy " << line << " " << e.what() << '\n';
					}
				}
			}
		}
	}

	/* //create a new manifest in the repo (for the repo's records)
	auto version = getLatestVersion(dst);
	//copy the new manifest we created in check-out but change it's name to a new version
	auto repo_manifest_path = getManifestPath(src, version + 1);

	try
	{
		fs::copy_file(new_manifest_path.string(), repo_manifest_path.string());
	}
	catch (fs::filesystem_error &e)
	{
		std::cout << "Could not copy new manifest to repo" << e.what() << '\n';
	} */

	manifest_file.close();
}

// checkin function creates a new version with files specified in src
void checkin(const std::string &src, const std::string &dst)
{
	auto version = getLatestVersion(dst);
	createVersion(src, dst, version + 1, true);
}

//merges changes from a directory back into the repo, creating a new version.
// repo is the path to the repo's manifest filename
// OR if label arg is being used to indicate repo version, repo is path to the repo's root folder
// and label is the label to select version
// target is path to target's root folder
void merge(const std::string &repo, const std::string &target, const std::string label)
{
	if (label == "")
	{
		std::size_t found = repo.rfind("manifest_");
		if (found != std::string::npos)
		{
			std::string repoPath = repo.substr(0, found);
			std::string manifestFileName = repo.substr(found);
			continueMerge(repoPath, target, manifestFileName);
		}
		else
		{
			std::cerr << "merge arguments do not contain manifest filename or label!" << '\n';
		}
	}
	else
	{
		fs::path manifest_path = findManifestByLabel(repo, label);
		if (manifest_path.empty())
		{
			std::cerr << "Label does not exist!" << '\n';
		}
		else
		{
			continueMerge(repo, target, manifest_path.filename().string());
		}
	}
}

//utility function for merge function

void continueMerge(const std::string &repo, const std::string &target, const std::string manifestFileName)
{
	std::cout << "\ncontinueMerge: " << repo << ", " << target << ", " << manifestFileName << '\n';

	//open repo manifest file
	std::string repoManifestPath = repo + manifestFileName;
	std::ifstream repo_manifest_file(repoManifestPath);
	if (!repo_manifest_file.is_open())
	{
		std::cerr << "Could not open repo manifest file:" + repoManifestPath << '\n';
	}

	//open target's most recent manifest file
	int versionNum = getMostRecentManifest(target);
	std::string targetManifestPath = getManifestPath(target, versionNum).string();
	std::ifstream target_manifest_file(targetManifestPath);
	if (!target_manifest_file.is_open())
	{
		std::cerr << "Could not open target manifest file:" + targetManifestPath << '\n';
	}

	std::string repo_line;
	bool repo_in_source_files = false;
	//will contain all the files paths relative to the repo
	std::vector<std::string> repoFiles;

	//iterate repo's manifest to compare source files in target's manifest
	while (std::getline(repo_manifest_file, repo_line))
	{
		if (!repo_in_source_files)
		{
			if (repo_line == "Source Files:")
			{
				repo_in_source_files = true;
			}
		}
		else
		{
			repoFiles.push_back(repo_line);
		}
	}

	std::string target_line;
	bool target_in_source_files = false;
	//will contain all the files paths relative to the target
	std::vector<std::string> targetFiles;

	while (std::getline(target_manifest_file, target_line))
	{
		if (!target_in_source_files)
		{
			if (target_line == "Source Files:")
			{
				target_in_source_files = true;
			}
		}
		else
		{
			targetFiles.push_back(target_line);
		}
	}

	//nested for loops to compare the two manifest source file entries
	for (auto repo_it = repoFiles.begin(); repo_it != repoFiles.end(); ++repo_it)
	{
		fs::path repo_file_path = *repo_it;
		std::string repo_artifact_path = repo_file_path.parent_path().string();
		std::string repo_artifact_name = repo_file_path.filename().string();
		bool foundInTarget = false;

		fs::path target_file_path;
		std::string target_artifact_path;
		std::string target_artifact_name;
		for (auto target_it = targetFiles.begin(); target_it != targetFiles.end(); ++target_it)
		{
			target_file_path = *target_it;
			target_artifact_path = target_file_path.parent_path().string();
			target_artifact_name = target_file_path.filename().string();

			if (repo_artifact_path == target_artifact_path)
			{
				foundInTarget = true;
				std::cout << "\nMATCH: " << repo_file_path << ", " << target_file_path << "\n";
				break;
			}
		}
		if (foundInTarget)
		{
			if (repo_artifact_name != target_artifact_name)
			{ //file versions are different (conflict)
				std::cout << "\nCONFLICT: \n"
						  << "\trepo_file_path: " << repo_file_path << '\n'
						  << "\ttarget_artifact_name: " << target_file_path << '\n';

				fs::path ancestorManifestPath = getMostRecentCommonAncestor(repoManifestPath, targetManifestPath);
				//TODO: copy the artifact version of the {{repo_artifact_path}} file specified by ancestorManifestPath
				//TODO: add _MR, _MT, and _MG versions to conflicted target file
				std::string root_target_path = target + "\\";
				std::string root_repo_path = repo;

				std::string target_path = root_target_path + target_artifact_path + "\\";
				std::string target_file_relative = target_file_path.string();
				std::string repo_file_relative = repo_file_path.string();
				std::string mt_path = root_target_path + removeExtension(target_file_relative) + "_MT.txt";
				std::string mr_path = root_target_path + removeExtension(repo_file_relative) + "_MR.txt";

				fs::rename(root_target_path + target_file_relative, mt_path);

				fs::copy(root_repo_path + repo_file_relative, target_path);
				fs::rename(target_path + repo_artifact_name, mr_path);

				std::ifstream ancestorManifest(ancestorManifestPath);
				std::string line;
				std::string original_repo_path;
				while (std::getline(ancestorManifest, line))
				{
					if (line.find("Check Out") != std::string::npos)
					{
						size_t mid_point = line.find(':', 23);
						original_repo_path = line.substr(21, mid_point - 22);
					}
					else if (original_repo_path.empty())
					{
						original_repo_path = ancestorManifestPath.parent_path().string();
					}

					if (line.find(repo_artifact_path) != std::string::npos)
					{
						fs::path ancestor_file_path = line;
						std::string ancestor_file_name = ancestor_file_path.filename().string();
						std::string mg_path = root_target_path + removeExtension(line) + "_MG.txt";
						fs::copy(original_repo_path + "\\" + line, target_path);
						fs::rename(target_path + ancestor_file_name, mg_path);
					}
				}
			}
		}
	}
	//TODO: add repo_file_path to target.
	//TODO: create new manifest in target
}

// Helper
std::string removeExtension(const std::string &file_name)
{
	std::string ext = ".txt";
	size_t pos = file_name.rfind(ext);

	if (pos == std::string::npos)
		return file_name;
	if (pos == 0)
		return file_name;

	return file_name.substr(0, pos);
}