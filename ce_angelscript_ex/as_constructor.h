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

#ifndef AS_CONSTRUCTOR_H
#define AS_CONSTRUCTOR_H
#include "as_doc.h"
#include "as_parameter.h"

// as_constructor is a constructor function of a class
class as_constructor : public as_doc {
public:
   as_constructor(cf_syslib::xml_node& node);
   as_constructor(const std::string& signature, bool verified,  std::shared_ptr<as_description> descr = 0);
   virtual ~as_constructor();

   // unverify this constructor and all descendants
   void unverify();

   // return raw signature as defined by asIScriptFunction::GetDeclaration
   std::string  signature() const { return m_signature; }

   // find a parameter by index (may return 0)
   std::shared_ptr<as_parameter> lookup_parameter(size_t index);

   // key returns the signature string, but with return type and all white space removed
   std::string key() const;

   // return number of parameters
   size_t size() const { return m_params.size(); }

   // compute key from signature
   static std::string key(size_t istart, const std::string& signature);

   // add constructor parameter
   void add_parameter(std::shared_ptr<as_parameter> param);

   // export this to XML as child under xml_parent
   cf_syslib::xml_node to_xml(cf_syslib::xml_node& xml_parent);

   // write this to doxygen header file
   void write_header(std::ostream& hfile);

   // add XML_TODO description items where descriptions are missing, according to level (see as_xml)
   void add_xml_todo(as_xml* factory, size_t level);

private:
   std::string                                m_signature; // angelscript signature as returned by asIScriptFunction::GetDeclaration
   std::vector<std::shared_ptr<as_parameter>> m_params;
};

#endif // AS_CONSTRUCTOR_H
