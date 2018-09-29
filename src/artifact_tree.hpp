// Author: Gabriel Martinez 1212gmartinez@csu.fullerton.edu
// This file contains the class definitions for artifact, artifactContainer, and folder.
// Currently these classes are not used but will be useful when adding more features

#pragma once
#ifndef artifact_tree_h
#define artifact_tree_h


struct artifact {
public:
	artifact(std::string artID = "", FILE *file = nullptr) : artID_(artID), file_(file) {}


	friend std::ostream& operator<<(std::ostream& os, const artifact& a) {
		return os << a.artID_;
	}

	std::string artID_;
	FILE *file_;
};

struct artifactContainer {
public:
	artifactContainer(std::string name = "", std::vector<artifact *> artifacts = std::vector<artifact *>()) :
		name_(name), artifacts_(artifacts) {}

	friend std::ostream& operator<<(std::ostream& os, const artifactContainer& a) {
		return os << a.name_;
	}

	std::string name_;
	std::vector<artifact *> artifacts_;

};

struct folder {
public:
	folder(std::string name = "", std::vector<folder *> children = std::vector<folder *>(), std::vector<artifactContainer *> artifactContainers = std::vector<artifactContainer *>()) :
		name_(name), children_(children), artifactContainers_(artifactContainers) {}


	void dispayTree(int layer = 1) {
		folder current = *this;
		std::cout << current.name_ << " {files:[";
		for (auto it = current.artifactContainers_.begin(); it != current.artifactContainers_.end(); ++it) {
			std::cout << **it << ", ";
		}

		std::cout << "]}\n";
		for (auto it = current.children_.begin(); it != current.children_.end(); ++it) {
			if (layer > 1) {
				std::cout << std::setfill(' ') << std::setw((layer - 1) * 4) << std::left << "|";
			}
			std::cout << std::setfill('-') << std::setw(4) << std::left << "|";

			(*it)->dispayTree(layer + 1);
		}
	}

	friend std::ostream& operator<<(std::ostream& os, const folder& f) {
		return os << "{\n   name: " << f.name_ << "\n   files: " <<
			f.artifactContainers_.size() << "\n   children: " << f.artifactContainers_.size() << "\n}";

	}

	std::string name_;
	std::vector<folder *> children_;
	std::vector<artifactContainer *> artifactContainers_;
};

#endif // file_tree_h
