#ifndef RECURSIVE_FILE_SEARCH_H
#define RECURSIVE_FILE_SEARCH_H

#include "cf_syslib_config.h"
#include <vector>
#include <string>

// search recursively under dir_path after files matching mask, return full paths
// max_recurse = # of recursion levels below root path (-1 means any)
CF_SYSLIB_PUBLIC
std::vector<std::string> recursive_file_search(const std::string& root_path, const std::string& file_mask, int max_recurse = -1);

#endif // RECURSIVE_FILE_SEARCH_H
