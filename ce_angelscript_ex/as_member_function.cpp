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

#include "as_member_function.h"
#include "as_parameter.h"
#include "as_return.h"
#include "as_xml.h"

std::set<std::string> as_member_function::m_export_filter;

as_member_function::as_member_function(const std::string& signature,  const std::string& name, bool verified, std::shared_ptr<as_description> descr)
: as_doc(verified,descr)
, m_signature(signature)
, m_name(name)
{}

as_member_function::as_member_function(cf_syslib::xml_node& node)
: as_doc(node)
{
   m_signature = node.get_property("signature","as_error");
   m_name      = node.get_property("name","as_error");
   for(auto i=node.begin(); i!=node.end(); i++) {
      cf_syslib::xml_node sub(i);
      if(!sub.is_attribute_node()) {
         if(sub.tag() == "as_parameter") m_params.push_back(std::make_shared<as_parameter>(sub));
         if(sub.tag() == "as_return") m_return = std::make_shared<as_return>(sub);
      }
   }
}

as_member_function::~as_member_function()
{}

void as_member_function::add_parameter(std::shared_ptr<as_parameter> param)
{
   m_params.push_back(param);
}

void as_member_function::add_return(std::shared_ptr<as_return> param)
{
   m_return = param;
}

cf_syslib::xml_node as_member_function::to_xml(cf_syslib::xml_node& xml_parent)
{
   cf_syslib::xml_node xml_this = xml_parent.add_child(as_typeid(this));
   as_doc::to_xml(xml_this);

   xml_this.add_property("signature",m_signature);
   xml_this.add_property("name",m_name);
   for(auto& par : m_params) {
      par->to_xml(xml_this);
   }
   if(m_return.get())m_return->to_xml(xml_this);

   return xml_this;
}


std::string as_member_function::key() const
{
   return key(m_signature);
}

std::string as_member_function::key(const std::string& signature)
{
   std::string key_sig;
   std::vector<std::string> tokens;
   as_description::tokenize(signature," ",tokens);
   for(size_t i=0;i<tokens.size();i++) {
      key_sig+=tokens[i];
   }
   return key_sig;
}


std::shared_ptr<as_parameter> as_member_function::lookup_parameter(size_t index)
{
   std::shared_ptr<as_parameter> par;
   if(index < m_params.size()) par = m_params[index];
   return par;
}

void as_member_function::unverify()
{
   set_verified(false);
   for(auto& par : m_params) {
      par->unverify();
   }
   if(m_return.get())m_return->unverify();
}


void as_member_function::write_header(std::ostream& hfile)
{
   if(m_export_filter.find(m_name) == m_export_filter.end()) {

      if(auto descr = description())  descr->write_header(hfile);

      std::vector<std::string> lines;
      for(auto& par : m_params) {
         lines.push_back(par->doxy_string());
      }
      std::string ret = m_return->doxy_string();
      if(ret.length()>0)lines.push_back(ret);

      if(lines.size() > 0) {
         size_t last_line = lines.size()-1;
         for(size_t i=0; i<lines.size(); i++) {
            if(i==        0) hfile << "   /*! " << lines[i];
            else             hfile << "   \\n " << lines[i];
            if(i==last_line) hfile << " */";
            hfile << std::endl;
         }
      }

      // doxygen is not able to handle angelscript handles on return values,
      // so instead we fake it by writing it as a C++ pointer
      // and later do a global replace from * to @ in the henerated HTML files.
      // Linux:
      //    rpl  " *&#160;" "@&#160;" *.html
      //
      // more or less: replace " * " with "@ "
      //
      size_t ipos = m_signature.find(' ');
      std::string return_type = m_signature.substr(0,ipos);
      size_t ip_ref = return_type.find("@");
      if(ip_ref != std::string::npos)return_type[ip_ref]='*';

      hfile << "  " << return_type << ' ' << as_xml::fix_array_types(m_signature.substr(ipos)) << ';' << std::endl << std::endl;
   }
}

void as_member_function::add_xml_todo(as_xml* factory, size_t level)
{
   if(level==2) {
      auto descr = description();
      if(!descr.get()) {
         set_description(std::make_shared<as_description>("XML_TODO"));
      }
   }
   if(level > 2) {
      for(auto p : m_params) p->add_xml_todo(factory,level);
   }
}
