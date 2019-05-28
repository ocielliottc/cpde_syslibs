#ifndef REPLACE_SUBSTRING_H
#define REPLACE_SUBSTRING_H

#include "cf_syslib_config.h"
#include <string>

// replace all instances of oldString with newString in line

// most efficient version with in-place replacement
CF_SYSLIB_PUBLIC
void replace_substring(std::string& line, const std::string& oldString, const std::string& newString);

// convenience variant retuns the modified string. Involves an extra copy of the input string
CF_SYSLIB_PUBLIC
std::string replace_substring_copy(const std::string& line, const std::string& oldString, const std::string& newString);

// replace string in given file, throw exception if file can not be opened for both read and write
CF_SYSLIB_PUBLIC
void replace_substring_in_file(const std::string& file_path, const std::string& oldString, const std::string& newString);

#endif // REPLACE_SUBSTRING_H
