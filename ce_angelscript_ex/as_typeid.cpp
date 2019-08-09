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
#include "as_typeid.h"


#ifndef  _MSC_VER
    /// GCC implements the Itanium C++ ABI, specified here:
    /// http://www.codesourcery.com/public/cxx-abi/abi.html
    #include <cxxabi.h>
#endif

std::string as_class_name(const std::type_info& type)
{
   std::string name = type.name();

#ifdef _MSC_VER
   /// For MSVC we can demangle by simply erasing the initial "class " part of the raw name when the type is a class
   /// for non-classes we do nothing
   if(name.find_first_of("class ")==0) {
      name.erase(0,6);
   }
#else
   /// Linux GNU g++
   /// Using demangler API of the Itanium C++ ABI
   /// Example in http://groups.google.com/group/gnu.g++.help/browse_thread/thread/ff412fbab4fb4eca
   size_t len=0;
   int s=0;
   char* p = abi::__cxa_demangle(name.c_str(),0,&len,&s);
   name = p;
   // _cxa_demangle documentation says user is responsible for deallocating the memory
   std::free(p);
#endif
   return name;
}
