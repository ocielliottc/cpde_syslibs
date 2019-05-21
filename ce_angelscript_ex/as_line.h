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

#ifndef AS_LINE_H
#define AS_LINE_H

#include "cf_syslib/xml_node.h"

// as_line is a single line text, usually part of as_description
class as_line {
public:
   as_line(cf_syslib::xml_node& node);
   as_line(const std::string& text);
   virtual ~as_line();

   // export this to XML as child under xml_parent
   cf_syslib::xml_node to_xml(cf_syslib::xml_node& xml_parent);

   const std::string& text() const { return m_text; }

private:
   std::string m_text;
};

#endif // AS_LINE_H
