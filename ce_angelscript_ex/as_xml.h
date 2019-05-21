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

#ifndef AS_XML_H
#define AS_XML_H

#include <map>
#include <set>
#include <string>
#include "as_class.h"
#include <ostream>
class asIScriptEngine;
class asITypeInfo;
class asIScriptFunction;
class as_class;

class as_xml {
public:
   as_xml(const std::string& appnam);
   virtual ~as_xml();

   std::string appnam() const { return m_appnam; }

   // unverify all contents
   void unverify();

   // fill data from script engine
   void from_script_engine(asIScriptEngine* engine);


   // look up a class by type name (may return null)
   std::shared_ptr<as_class> lookup_class(const std::string& type_name, bool verified);

   // insert a class
   void insert(std::shared_ptr<as_class> type) { m_types[type->name()] = type; }

   // write to xml stream
   void write_xml(std::ostream& xml);

   // read from xml stream
   void read_xml(std::istream& xml);

   // write header file to be readable by doxygen
   void write_header(std::ostream& hfile);

   // beautify doxygen header file array syntax to use array<type> syntax instead of type[] syntax
   static std::string fix_array_types(const std::string& signature);

   // to replace double[] into array<double>, call as_xml::add_array_type("double");
   static void add_array_type(const std::string& type) { m_array_types.insert(type); }

   // add XML_TODO items for missing descriptions
   // level 1 : class descriptions
   // level 2 : constructor/member function descriptions
   // level 3 : parameter secriptions
   // this applies to the specified type only, or to all types if class_type="*"
   void add_xml_todo(size_t level, const std::string& class_type);


protected:

   // add constructors to the class instance
   void from_script_engine_constructors(asIScriptEngine* engine, asITypeInfo* type, std::shared_ptr<as_class> class_type);
   void from_script_engine_parameters_constr(asIScriptEngine* engine, asIScriptFunction* func, std::shared_ptr<as_constructor> as_constr);

   // add member functions to the class instance
   void from_script_engine_member_functions(asIScriptEngine* engine, asITypeInfo* type, std::shared_ptr<as_class> class_type);
   void from_script_engine_parameters(asIScriptEngine* engine, asIScriptFunction* func, std::shared_ptr<as_member_function> as_mem_fun);

   void resolve_base_types();

private:
   std::string                                     m_appnam; // name of application
   std::map<std::string,std::shared_ptr<as_class>> m_types;  // using a map, we get alphabetic sorting

   static std::set<std::string>                    m_array_types;  // for "fix_array_types"
};

#endif // AS_XML_H
