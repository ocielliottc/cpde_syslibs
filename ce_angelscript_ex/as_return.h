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

#ifndef AS_RETURN_H
#define AS_RETURN_H

#include "as_doc.h"

// as_return is a return value from a function
class as_return : public as_doc {
public:
   as_return(cf_syslib::xml_node& node);
   as_return(const std::string& type,  bool verified, std::shared_ptr<as_description> descr = 0);
   virtual ~as_return();

   void set_type(const std::string& type) { m_type = type; }
   std::string type() const { return m_type; }

   cf_syslib::xml_node to_xml(cf_syslib::xml_node& xml_parent);

   void unverify();

   std::string doxy_string();

   void add_xml_todo(as_xml* factory, size_t level);

private:
   std::string m_type;    // type name of parameter, e.g. "double"
};

#endif // AS_RETURN_H
