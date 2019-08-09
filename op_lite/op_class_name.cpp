#include "op_class_name.h"

#ifndef  _MSC_VER
    /// GCC implements the Itanium C++ ABI, specified here:
    /// http://www.codesourcery.com/public/cxx-abi/abi.html
    #include <cxxabi.h>
#endif

std::string op_class_name(const std::string& typeid_name)
{
   std::string name = typeid_name;
#ifdef _MSC_VER
   /// For MSVC we can demangle by simply erasing the "class " part of the raw name
   name.erase(0,6);
#else
   /// Linux GNU g++
   /// Using demangler API of the Itanium C++ ABI
   /// Example in http://groups.google.com/group/gnu.g++.help/browse_thread/thread/ff412fbab4fb4eca
   size_t len=0;
   int s=0;
   char* p = abi::__cxa_demangle(typeid_name.c_str(),0,&len,&s);
   name = p;
   // _cxa_demangle documentation says user is responsible for deallocating the memory
   std::free(p);
#endif
   return name;
}
