#ifndef AS_DOC_H
#define AS_DOC_H

#include <vector>
#include <string>
#include <memory>
#include "cf_syslib/xml_node.h"
#include "as_typeid.h"
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

#include "as_description.h"
class as_xml;

// as_doc is the base class for several types, it manages description data
class as_doc {
public:
   as_doc(bool verified, std::shared_ptr<as_description> descr);
   as_doc(cf_syslib::xml_node& node);
   virtual ~as_doc();

   // unverify this object and all descendants
   virtual void unverify() = 0;

   // provide read & write access to description
   std::shared_ptr<as_description> description() { return m_descr; }

   // check verification flag
   bool is_verified() const { return m_verified; }

   // set verification flag
   void set_verified(bool verified = true) { m_verified = verified; }

   // set new description
   void set_description(std::shared_ptr<as_description> descr) { m_descr = descr; }

   // export this object to XML DOM tree, under xml_parent
   virtual cf_syslib::xml_node to_xml(cf_syslib::xml_node& xml_parent) = 0;

   // add XML_TODO items for missing descriptions
   // level 0 : class descriptions
   // level 1 : constructor/member function descriptions
   // level 2 : parameter secriptions
   // if type is specified, this applies to the specified type only
   virtual void add_xml_todo(as_xml* factory, size_t level) = 0;

private:
   bool                            m_verified;  // true when verified by script engine
   std::shared_ptr<as_description> m_descr;
};

#endif // AS_DOC_H
