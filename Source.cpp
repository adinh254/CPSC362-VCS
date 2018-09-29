// Filesystem library requires C++17
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include<array>
#include <regex>

namespace fs = std::filesystem;

// Calculates content of artifact to get ID.
std::string getArtifactID( const fs::path& path, std::fstream& myFile );

// Renames file name to artifact ID.
void renameArtifact( const fs::path& path );

// Copies original project tree and creates a new repo at the destination with the entire project tree inside.
void createRepo( const::std::string& root, const std::string& dst );



// Array and vector to store weighted numbers and file content.
// For loop to calculate check sum.
std::string getArtifactID( const fs::path& path, std::fstream& myFile )
{
	std::array <int, 5> weight = { 1, 3, 5, 7, 17 };
	std::vector<char> artifact;

	char ch;
	unsigned int fileSize = 0;
	unsigned int sum = 0; // Unsigned prevents integer overflow.
	std::string artID = "";

	// Get file content.
	while( myFile.get( ch ) )
	{
		artifact.push_back( ch );
		fileSize++;
	}

	// Calculate check sum.
	unsigned int weightIndex = 0;
	for( auto& c : artifact )
	{
		sum += c * weight[weightIndex];
		if( weightIndex == 4 )
			weightIndex = 0;
		else
			weightIndex++;
	}

	// https://en.cppreference.com/w/cpp/filesystem/path/parent_path
	// https://en.cppreference.com/w/cpp/filesystem/path/extension
	artID = path.parent_path().string() + "/" + std::to_string( sum ) + "-L" + std::to_string( fileSize ) + path.extension().string();

	// Close file to enable renaming.
	myFile.close();

	return artID;
}

void renameArtifact( const fs::path& path )
{
	std::fstream myFile;
	myFile.open( path );


	if( std::rename( path.string().c_str(), getArtifactID( path, myFile ).c_str() ) )
		std::perror( "Error Renaming" );
}

// Create new repo directory.
// Recursively iterate through entire project tree and copy contents to destination repo.
// Case to check if path is a file.
void createRepo( const::std::string& root, const std::string& dst )
{
	// New repo directory
	fs::path rootLast = root;
	fs::path repo = dst + "/";
	repo += rootLast.filename();
	fs::create_directories( repo ); 
	
	// Recursively iterate through project tree.
	// https://en.cppreference.com/w/cpp/filesystem/recursive_directory_iterator
	for( auto iter = fs::recursive_directory_iterator( root ); iter != fs::recursive_directory_iterator(); ++iter )
	{
		std::error_code ec;

		// Reassign destination path to current project tree path.
		// https://en.cppreference.com/w/cpp/filesystem/relative
		fs::path dstContent = repo.string() + "/";
		dstContent += fs::relative( iter->path(), root );

		fs::create_directory( dstContent, ec );
		if( ec )
		{
			std::cerr << "Error creating directory: " << dstContent << "\nFileSystem Error: " <<  ec.message() << '\n';
		}

		// File Check Case
		if( fs::is_regular_file( iter->path() ))
		{
			// Copy file and contents to current repo directory.
			copy( iter->path(), dstContent, ec );
			if( ec )
			{
				std::cerr << "Error while copying: " << dstContent << "\nFileSystem Error: " << ec.message() << '\n';
			}

			// Pointer to first file of current repo path.
			fs::directory_iterator firstEntry( dstContent ); 
			renameArtifact( firstEntry->path() );
		}
	}
}

// Menu to input root path and destination path.
// Test Folders need to be in same directory as source.
int main(int argc, char* argv[]) 
{
	std::string rootFolderName;
	std::string destinationFolderName;
	std::cout << "Enter your Folder Name: ";
	std::cin >> rootFolderName;
	std::cout << "Enter your destination repo folder name: ";
	std::cin >> destinationFolderName;

	std::string rootPath = fs::current_path().string() + "/" + rootFolderName;
	std::string destinationPath = fs::current_path().string() + "/" + destinationFolderName;
	createRepo( rootPath, destinationPath );

	return 0;
}
