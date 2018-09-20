#pragma once
#ifndef artifact_tree_h
#define artifact_tree_h


struct artifact {
public:
	artifact(std::string artID = "", FILE *file = nullptr) : artID_(artID), file_(file) {}
	void setArtID() {

	}


	friend std::ostream& operator<<(std::ostream& os, const artifact& a) {
		return os << a.artID_;
	}

	std::string artID_;
	FILE *file_;
};

struct folder {
public:
	folder(std::string name = "", std::vector<folder *> children = std::vector<folder *>(), std::vector<artifact *> artifacts = std::vector<artifact *>()) :
		name_(name), children_(children), artifacts_(artifacts) {}


	void dispayTree(int layer = 1) {
		folder current = *this;
		std::cout << current.name_ << " [artifacts: ";
		if (current.artifacts_.empty()) {
			std::cout << "NONE";
		}
		else {
			for (auto it = current.artifacts_.begin(); it != current.artifacts_.end(); ++it) {
				std::cout << **it << ", ";
			}
		}
		std::cout << "]\n";
		for (auto it = current.children_.begin(); it != current.children_.end(); ++it) {
			if (layer > 1) {
				std::cout << std::setfill(' ') << std::setw((layer - 1) * 4) << std::left << "|";
			}
			std::cout << std::setfill('-') << std::setw(4) << std::left << "|";

			(*it)->dispayTree(layer + 1);
		}
	}

	friend std::ostream& operator<<(std::ostream& os, const folder& f) {
		return os << "{\n   name: " << f.name_ << "\n   artifacts: " <<
			f.artifacts_.size() << "\n   children: " << f.children_.size() << "\n}";

	}

	std::string name_;
	std::vector<folder *> children_;
	std::vector<artifact *> artifacts_;
};

#endif // file_tree_h
