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

#ifndef AS_INITIALIZER_VECTOR_H
#define AS_INITIALIZER_VECTOR_H

#include <vector>
#include <stdexcept>
#include <string.h>
#include "ce_angelscript/ce_angelscript.h"

// basic initializer list encapsulation for fundamental types, i.e. T=double,int etc.
// A scripting class can be declared with as_initializer_vector<T> as constructor parameter to simplify.
// If the initializer list must be have a required length, use the first constructor

template <class T>
class as_initializer_vector {
public:

   // this constructor throws exception if initializer list is not of required length
   as_initializer_vector(size_t required_length, void* initlist);

   // this constructor accepts any length
   as_initializer_vector(void* initlist);

   // destructor
   virtual ~as_initializer_vector();

   // size of vector after construction
   size_t size() const { return m_data.size(); }

   // read only operator
   const T operator[](size_t index) const { return m_data[index]; }

private:
   std::vector<T> m_data;
};

template <class T>
as_initializer_vector<T>::as_initializer_vector(size_t required_length, void* initlist)
{
   // Determine the length of the initializer list
   asUINT length = *(asUINT*)initlist;
   if(length != required_length) {
      ::memset(((asUINT*)initlist), 0, 1+length*sizeof(T) );
      throw std::logic_error("Initializer list has length="+ std::to_string(length) + ", required length=" + std::to_string(required_length));
   }

   m_data.reserve(length);
   T* value = reinterpret_cast<T*>(((asUINT*)initlist)+1);
   for(size_t i=0;i<length;i++) {
      m_data.push_back(*value++);
   }

   // zero the input list completely, this avoids segfault in angelscript later
   ::memset(((asUINT*)initlist), 0, 1+length*sizeof(T) );
}

template <class T>
as_initializer_vector<T>::as_initializer_vector(void* initlist)
{
   // Determine the length of the initializer list
   asUINT length = *(asUINT*)initlist;
   m_data.reserve(length);
   T* value = reinterpret_cast<T*>(((asUINT*)initlist)+1);
   for(size_t i=0;i<length;i++) {
      m_data.push_back(*value++);
   }

   // zero the input list completely, this avoids segfault in angelscript later
   ::memset(((asUINT*)initlist), 0, 1+length*sizeof(T) );
}

template <class T>
as_initializer_vector<T>::~as_initializer_vector()
{}


#endif // AS_INITIALIZER_VECTOR_H
