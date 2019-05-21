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

#ifndef AS_PARAMETER_H
#define AS_PARAMETER_H

#include "as_doc.h"

// as_parameter is a parameter in a function call
class as_parameter : public as_doc {
public:
   as_parameter(cf_syslib::xml_node& node);
   as_parameter(const std::string& type, const std::string& name, const std::string& defval,  bool verified, std::shared_ptr<as_description> descr = 0);
   virtual ~as_parameter();

   // unverify this parameter and all descendants
   void unverify();

   void set_name(const std::string& name) { m_name = name; }
   void set_type(const std::string& type) { m_type = type; }
   void set_defval(const std::string& defval) { m_defval = defval; }

   std::string type() const { return m_type; }
   std::string name() const { return m_name; }
   std::string defval() const { return m_defval; }

   cf_syslib::xml_node to_xml(cf_syslib::xml_node& xml_parent);

   std::string doxy_string();

   void add_xml_todo(as_xml* factory, size_t level);

private:
   std::string m_type;    // type name of parameter, e.g. "double"
   std::string m_name;    // parameter name
   std::string m_defval;  // default value or empty string if no default
};

#endif // AS_PARAMETER_H
