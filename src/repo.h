#ifndef REPO_H
#define REPO_H

#include <string>

// Copies original project tree and creates a new repo at the destination with the entire project tree inside.
void createRepo(const std::string& root, const std::string& dst);
void createVersion(const std::string& root, const std::string& dst, int version, bool checkin = false);

int getLatestVersion(const std::string &src);

void continueCheckout(const std::string &src, const std::string &dst, const std::string &manifest,
                            const std::string &label);

void checkout(const std::string& src, const std::string& dst, const std::string& manifest_info);
void checkin(const std::string& src, const std::string &dst);

void merge(const std::string& repo, const std::string &target, const std::string label);
void continueMerge(const std::string& repo, const std::string &target, const std::string repoVersion);

#endif 