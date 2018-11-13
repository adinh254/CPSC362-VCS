#ifndef REPO_H
#define REPO_H

#include <string>

// Copies original project tree and creates a new repo at the destination with the entire project tree inside.
void createRepo(const std::string& root, const std::string& dst);

void checkout(const std::string& src, const std::string &dst);

#endif 