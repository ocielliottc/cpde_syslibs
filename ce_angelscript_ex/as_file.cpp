// BeginLicense:
// Part of: cpde_syslibs - Cross Platform Development Environment, system libraries
// Copyright (C) 2017 Carsten Arnholm
// All rights reserved
//
// Permission is granted to anyone to use this software for any
// purpose, including commercial applications, and to alter it and
// redistribute it freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you
//    must not claim that you wrote the original software. If you use
//    this software in a product, an acknowledgment in the product
//    documentation would be appreciated.
//
// 2. Altered source versions must be plainly marked as such, and
//    must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
// This file is provided "AS IS" with NO WARRANTY OF ANY KIND,
// INCLUDING THE WARRANTIES OF DESIGN, MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE.
// EndLicense:


#include "as_file.h"
#ifdef _WIN32
   #include "Shlwapi.h"
   #pragma comment (lib,"shlwapi.lib")
#else
   #include <sys/stat.h>
#endif

bool as_file::file_exists(const std::string& path)
{
   #ifdef _WIN32
      return (TRUE == PathFileExists(path.c_str()));
   #else
      struct stat fileStat;
      if(stat(path.c_str(), &fileStat) )return false;
      if(!S_ISREG(fileStat.st_mode)) return false;
      return true;
   #endif
}

bool as_file::dir_exists(const std::string& path)
{
   #ifdef _WIN32
      DWORD ftyp = GetFileAttributesA(path.c_str());
      if (ftyp == INVALID_FILE_ATTRIBUTES) return false;  //something is wrong with your path!
      if (ftyp & FILE_ATTRIBUTE_DIRECTORY) return true;   // this is a directory!
      return false;
   #else
      struct stat fileStat;
      if(stat(path.c_str(), &fileStat) )return false;
      if(!S_ISDIR(fileStat.st_mode)) return false;
      return true;
   #endif
}



