Version Control System

This is a simple version control system, a project created for
California State University, Fullerton's Software Engineering class.


Class number: CPSC362-01/02 12846/12847
Project number: 2
Project name: Version control system
Team name: KGA
Team members: Kedyn Macedonio, Gabriel Martinez, Andrew Dinh


Intro:
	This project currently implements a create, check-out, check-in,
	label, functionalities. The create functiononality copies the
	contents of one source subtree to another destination subtree
	using the c++17 filesystem library's file iteration. The
	check-out functiononality allows the user to retrive all files
	related to a specific version of the repository. The check-in
	functionality allows to update a version from the check-out
	functionality. The label functionality allows the user to name
	a version, that that it may be a reference for later use, including
	for the check-out functionality.

Contents:  
	- artifact.h: simple header to be included in source files

	- artifact.cpp: contains getArtifactID and renameArtifact
	 functions

	- main.cpp: the main file of the project holds everything
	 together

	- repo.h: simple header to be included in source files

	- repo.cpp: contains create, check-out, check-in, and label
		 functions.

Dependencies:

	Created using Windows using C++. 
	Project contains libraries: 
		- iostream 
		- string
		- vector 
		- filesystem (NOTE: THIS IS A C++17 LIBRARY)
		- fstream
		- array

External Requirements:

	This project requires gcc 8.2 or any compiler that contains the c++17
	filesystem library.

Setup and Installation:
	
	You must compile the files included in the Contents section of this file.

Tests and Samples:

	VCS-BINARY-EXEC create test/mypt myrepos/mypt

        After executing the previous command a folder myrepos will be created under
        the root folder. Within myrepos folder the following structure of
        folders/files will be found within it:

            mypt/
                ht.txt/
                    72-L1.txt
                manifest_1.txt

	VCS-BINARY-EXEC create test/mypt2 myrepos/mypt2

        After executing the previous command a folder myrepos will be created under
        the root folder. Within myrepos folder the following structure of
        folders/files will be found within it:

            mypt2/
                ht.txt/
                    72-L1.txt
                Stuff/
                    goodbye.txt/
                        3615-L10.txt
                    hello.txt/
                        7266-L12.txt
                manifest_1.txt

	VCS-BINARY-EXEC create test/mypt3 myrepos/mypt3

        After executing the previous command a folder myrepos will be created under
        the root folder. Within myrepos folder the following structure of
        folders/files will be found within it:

            mypt3/
                ht.txt/
                    72-L1.txt
                src/
                    darn.fool/
                        49351-L80.fool
                    main.fool/
                        13737-L22.fool
                manifest_1.txt

	VCS-BINARY-EXEC create test/mypt4 myrepos/mypt4

        After executing the previous command a folder myrepos will be created under
        the root folder. Within myrepos folder the following structure of
        folders/files will be found within it:

            mypt4/
                src/
                    headers/
                        tools.h/
                            36175-L65.h
                    implementations/
                        tools.cpp/
                            67785-L124.cpp
                    main.cpp/
                        55952-L98.cpp
                manifest_1.txt

	VCS-BINARY-EXEC create test/mypt5 myrepos/mypt5

        After executing the previous command a folder myrepos will be created under
        the root folder. Within myrepos folder the following structure of
        folders/files will be found within it:

            mypt5/
                css/
                    stylesheet.css/
                        19422-L37.css
                images/
                    hello-world.jpg/
                        23775-L248.jpg
                index.html/
                    186866-L339.html
                javascript/
                    main.js/
                        11411-L21.js
                manifest_1.txt

	VCS-BINARY-EXEC label myrepos/manifest_1.txt version1

		After executing the previous command a version1 label is added to the manifest file
        with the name or label specified in the first argument. The contents of manifest_1.txt:

            Label: version1
            Create Repo Arguments: ./test/mypt/ ./test/repos/mypt/
            Time of Command: 20:12:34 November 17, 2018
            Source Files:
            hx.txt/72-L1.txt

	VCS-BINARY-EXEC label version1 KGA Version

        Note: This must be executed on the myrepos/mypt/ directory.

		After executing the previous command a "KGA Version" label is added to the manifest file
        with the name or label specified in the first argument. The contents of manifest_1.txt:

            Label: KGA Version
            Label: version1
            Create Repo Arguments: ./test/mypt/ ./test/repos/mypt/
            Time of Command: 20:12:34 November 17, 2018
            Source Files:
            hx.txt/72-L1.txt

    VCS-BINARY-EXEC check-out myrepos/mypt checkouts/mypt/ version1
    
        After executing the previous command the folder mypt will have the following structure:

            mypt/
                hx.txt
                manifest_1.txt

        Along with this a new manifest file will be created on myrepos/mypt called "manifest_2.txt"

    VCS-BINARY-EXEC check-out myrepos/mypt2 checkouts/mypt2 manifest_1.txt

        After executing the previous command the folder mypt will have the following structure:

            mypt2/
                Stuff/
                    goodbye.txt
                    hello.txt
                hx.txt
                manifest_1.txt

        Along with this a new manifest file will be created on myrepos/mypt2 called "manifest_2.txt"

    VCS-BINARY-EXEC check-out myrepos/mypt3 checkouts/mypt3 manifest_1.txt

        After executing the previous command the folder mypt will have the following structure:

            mypt3/
                ht.txt
                src/
                    darn.fool
                    main.fool
                manifest_1.txt

        Along with this a new manifest file will be created on myrepos/mypt3 called "manifest_2.txt"

    VCS-BINARY-EXEC check-out myrepos/mypt4 checkouts/mypt4 manifest_1.txt

        After executing the previous command the folder mypt will have the following structure:

            mypt4/
                src/
                    headers/
                        tools.h
                    implementations/
                        tools.cpp
                    main.cpp
                manifest_1.txt

        Along with this a new manifest file will be created on myrepos/mypt4 called "manifest_2.txt"

    VCS-BINARY-EXEC check-out myrepos/mypt5 checkouts/mypt5 manifest_1.txt

        After executing the previous command the folder mypt will have the following structure:

            mypt5/
                css/
                    stylesheet.css
                images/
                    hello-world.jpg
                javascript/
                    main.js
                index.html
                manifest_1.txt

        Along with this a new manifest file will be created on myrepos/mypt5 called "manifest_2.txt"

    VCS-BINARY-EXEC check-in checkouts/mypt myrepos/mypt

        After executing the previous command a folder myrepos will be created under
        the root folder. Within myrepos folder the following structure of
        folders/files will be found within it:

            mypt/
                ht.txt/
                    72-L1.txt
                manifest_1.txt
                manifest_2.txt
                manifest_3.txt

	VCS-BINARY-EXEC check-in checkouts/mypt2 myrepos/mypt2

        After executing the previous command a folder myrepos will be created under
        the root folder. Within myrepos folder the following structure of
        folders/files will be found within it:

            mypt2/
                ht.txt/
                    72-L1.txt
                Stuff/
                    goodbye.txt/
                        3615-L10.txt
                    hello.txt/
                        7266-L12.txt
                manifest_1.txt
                manifest_2.txt
                manifest_3.txt

	VCS-BINARY-EXEC check-in checkouts/mypt3 myrepos/mypt3

        After executing the previous command a folder myrepos will be created under
        the root folder. Within myrepos folder the following structure of
        folders/files will be found within it:

            mypt3/
                ht.txt/
                    72-L1.txt
                src/
                    darn.fool/
                        49351-L80.fool
                    main.fool/
                        13737-L22.fool
                manifest_1.txt
                manifest_2.txt
                manifest_3.txt

	VCS-BINARY-EXEC check-in checkouts/mypt4 myrepos/mypt4

        After executing the previous command a folder myrepos will be created under
        the root folder. Within myrepos folder the following structure of
        folders/files will be found within it:

            mypt4/
                src/
                    headers/
                        tools.h/
                            36175-L65.h
                    implementations/
                        tools.cpp/
                            67785-L124.cpp
                    main.cpp/
                        55952-L98.cpp
                manifest_1.txt
                manifest_2.txt
                manifest_3.txt

	VCS-BINARY-EXEC check-in checkouts/mypt5 myrepos/mypt5

        After executing the previous command a folder myrepos will be created under
        the root folder. Within myrepos folder the following structure of
        folders/files will be found within it:

            mypt5/
                css/
                    stylesheet.css/
                        19422-L37.css
                images/
                    hello-world.jpg/
                        23775-L248.jpg
                index.html/
                    186866-L339.html
                javascript/
                    main.js/
                        11411-L21.js
                manifest_1.txt
                manifest_2.txt
                manifest_3.txt


Features:

	Currently the program allows the user to:
		- create a repository for a project.
		- check-out a repository to create a new version.
		- check-in allows to add a new version to the repository
		- label allows to create/add a specific version name


Bugs:
	No current bugs found.
