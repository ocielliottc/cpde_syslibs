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

#ifndef AS_DESCRIPTION_H
#define AS_DESCRIPTION_H
#include <string>

#include "cf_syslib/xml_node.h"
#include "as_line.h"
#include <vector>

// as_description is a potentially multi-line description of a class, function, parameter etc.
class as_description {
public:
   as_description(cf_syslib::xml_node& node);
   as_description(const std::string& text = "");
   as_description(const as_line& line);
   virtual ~as_description();

   void add_line(const as_line& line);

   cf_syslib::xml_node to_xml(cf_syslib::xml_node& xml_parent);

   void write_header(std::ostream& hfile, size_t indent = 3);

   size_t size() const { return m_lines.size(); }
   const as_line& line(size_t i) const { return m_lines[i]; }

   static void tokenize(const std::string& input,
                        const std::string& delimiters,
                        std::vector<std::string>& tokens);

private:
   std::vector<as_line> m_lines;
};

#endif // AS_DESCRIPTION_H
