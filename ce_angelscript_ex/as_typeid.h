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

#ifndef AS_TYPEID_H
#define AS_TYPEID_H

#include <string>
#include <typeinfo>

/// return typeid string for any given class type_info
std::string as_class_name(const std::type_info& type);

/// return typeid string for any given class (template parameter)
template<class T> std::string as_typeid() { return as_class_name(typeid(T)); }
template<class T> std::string as_typeid(const T* object) { return as_class_name(typeid(*object)); }

#endif // AS_TYPEID_H
