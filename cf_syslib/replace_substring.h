#ifndef REPLACE_SUBSTRING_H
#define REPLACE_SUBSTRING_H

#include "cf_syslib_config.h"
#include <string>

// replace all instances of oldString with newString in line

// most efficient version with in-place replacement
CF_SYSLIB_PUBLIC
void replace_substring(std::string& line, std::string& oldString, std::string& newString);

// convenience variant retuns the modified string. Involves an extra copy of the input string
CF_SYSLIB_PUBLIC
std::string replace_substring(const std::string& line, std::string& oldString, std::string& newString);

#endif // REPLACE_SUBSTRING_H
