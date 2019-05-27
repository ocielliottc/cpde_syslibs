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
#include "as_typeid.h"
#include "as_doc.h"
#include <ostream>

as_description::as_description(cf_syslib::xml_node& node)
{
   for(auto i=node.begin(); i!=node.end(); i++) {
      cf_syslib::xml_node sub(i);
      if(!sub.is_attribute_node()) {
         if(sub.tag() == "as_line") m_lines.push_back(sub);
      }
   }
}

as_description::as_description(const std::string& text)
{
   if(text.length() > 0) {
      std::vector<std::string> tokens;
      tokenize(text,"\r\n",tokens);
      for(auto& line : tokens) {
         m_lines.push_back(line);
      }
   }
}

as_description::as_description(const as_line& line)
{
   m_lines.push_back(line);
}

as_description::~as_description()
{}

void as_description::add_line(const as_line& line)
{
   m_lines.push_back(line);
}

cf_syslib::xml_node as_description::to_xml(cf_syslib::xml_node& xml_parent)
{
   cf_syslib::xml_node xml_this = xml_parent.add_child(as_typeid(this));
   for(auto& line : m_lines) {
      line.to_xml(xml_this);
   }
   return xml_this;
}


void as_description::tokenize(const std::string& input,
                      const std::string& delimiters,
                      std::vector<std::string>& tokens)
{
   using namespace std;
   string::size_type last_pos = 0;
   string::size_type pos = 0;
   while(true) {
      pos = input.find_first_of(delimiters, last_pos);
      if( pos == string::npos ) {
         if(input.length()-last_pos > 0)tokens.push_back(input.substr(last_pos));
         break;
      }
      else {
         if(pos-last_pos > 0)tokens.push_back(input.substr(last_pos, pos - last_pos));
         last_pos = pos + 1;
      }
   }
}

void as_description::write_header(std::ostream& hfile, size_t indent)
{
   if(m_lines.size()>0) {
      std::string spaces;
      if(indent>0) spaces.resize(indent,' ');
      // first line has special meaning as "one liner"
      hfile << spaces << "/// " << m_lines[0].text() << std::endl;
      if(m_lines.size()>1) {
         size_t last_line = m_lines.size()-1;
         for(size_t i=1; i<m_lines.size(); i++) {
            if(i==        1) hfile << spaces << "/*! " << m_lines[i].text();
            else             hfile << spaces << "\\n " << m_lines[i].text();
            if(i==last_line) hfile << spaces << "*/";
            hfile << std::endl;
         }
      }
   }
}
