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

#ifndef AS_CLASS_H
#define AS_CLASS_H

#include "as_doc.h"
#include <map>
#include <set>
class as_constructor;
class as_member_function;
class as_xml;

// as_class represents a complete scripting class
class as_class : public as_doc {
   friend class as_xml;
public:
   as_class(cf_syslib::xml_node& node);
   as_class(const std::string& name, bool verified, std::shared_ptr<as_description> descr);
   virtual ~as_class();

   // unverify this class and all descendants
   void unverify();

   // look up constructor by type signature (may return null). Do not include return type in constructor signature
   std::shared_ptr<as_constructor>     lookup_constructor(const std::string& signature, bool verified);

   // look up member function by type signature (may return null). Do include return type in signature
   std::shared_ptr<as_member_function> lookup_member_function(const std::string& signature, bool verified);

   // set name of base class
   void set_base(const std::string& base) { m_base = base; }

   // return name of class
   std::string name() const { return m_name; }

   // return name of base class, or empty if none
   std::string base() const { return m_base; }

   // add constructor to the class
   void add_constructor(std::shared_ptr<as_constructor> constr);

   // add member function to the class
   void add_member_function(std::shared_ptr<as_member_function> mfun);

   // export this to XML as child under xml_parent
   cf_syslib::xml_node to_xml(cf_syslib::xml_node& xml_parent);

   // return true if the function signature was inherited
   bool function_inherited(as_xml* factory, std::shared_ptr<as_member_function> mfun);

   // write this to doxygen header file
   void write_header(as_xml* factory, std::ostream& hfile);

   // add a class name to the export filter, i.e. it will not be exported to header file
   static void add_export_filter(const std::string& type_name) { m_export_filter.insert(type_name); }

   // add XML_TODO description items where descriptions are missing, according to level (see as_xml)
   void add_xml_todo(as_xml* factory, size_t level);

protected:
   bool has_unique_base_type(const std::string& base_type);
   void add_base_candidate(std::string base_type);
   bool resolve_base_type(as_xml* factory);

private:
   std::string m_name;  // name of class
   std::string m_base;  // name of base class or empty string if none

   std::map<std::string,std::shared_ptr<as_constructor>>     m_constr;     // key = as_constructor::key()
   std::map<std::string,std::shared_ptr<as_member_function>> m_mem_funs;   // key = as_member_function::key()

   std::set<std::string> m_base_candidates; // only used temporarily during script engine processing
   static std::set<std::string> m_export_filter;  // names of classes that shall not be exported to header file
};

#endif // AS_CLASS_H
