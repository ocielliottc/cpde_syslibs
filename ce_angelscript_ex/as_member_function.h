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

#ifndef AS_MEMBER_FUNCTION_H
#define AS_MEMBER_FUNCTION_H
#include "as_doc.h"
#include "as_parameter.h"
#include "as_return.h"
#include <set>

// as_member_function is a member function of a class
class as_member_function : public as_doc {
public:
   as_member_function(cf_syslib::xml_node& node);
   as_member_function(const std::string& signature, const std::string& name, bool verified, std::shared_ptr<as_description> descr = 0);
   virtual ~as_member_function();

   // unverify this function and all descendants
   void unverify();

   std::string  name() const { return m_name; }

   // return raw signature as defined by asIScriptFunction::GetDeclaration
   std::string  signature() const { return m_signature; }

   // find a parameter by index (may return 0)
   std::shared_ptr<as_parameter> lookup_parameter(size_t index);
   std::shared_ptr<as_return>    lookup_return() { return m_return; }

   // key returns the signature string, but with all white space removed
   std::string key() const;

   // return number of parameters
   size_t size() const { return m_params.size(); }

   // compute key from signature
   static std::string key(const std::string& signature);

   void add_parameter(std::shared_ptr<as_parameter> param);
   void add_return(std::shared_ptr<as_return> param);

   cf_syslib::xml_node to_xml(cf_syslib::xml_node& xml_parent);

   void write_header(std::ostream& hfile);

   static void add_export_filter(const std::string& func_name) { m_export_filter.insert(func_name); }
   static bool is_filtered(const std::string& func_name) { return (m_export_filter.find(func_name) != m_export_filter.end()); }

   void add_xml_todo(as_xml* factory, size_t level);

private:
   std::string                                m_signature;  // angelscript signature as returned by asIScriptFunction::GetDeclaration
   std::string                                m_name;       // angelscript name returned by asIScriptFunction::GetName
   std::vector<std::shared_ptr<as_parameter>> m_params;
   std::shared_ptr<as_return>                 m_return;

   static std::set<std::string>               m_export_filter; // names of functions that shall not be exported to header file
};

#endif // AS_MEMBER_FUNCTION_H
