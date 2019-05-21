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

#include "as_class.h"
#include "as_constructor.h"
#include "as_member_function.h"
#include "as_xml.h"

std::set<std::string> as_class::m_export_filter;

as_class::as_class(cf_syslib::xml_node& node)
: as_doc(node)
{
   m_name = node.get_property("name","as_error");
   m_base = node.get_property("base","");

   for(auto i=node.begin(); i!=node.end(); i++) {
      cf_syslib::xml_node sub(i);
      if(!sub.is_attribute_node()) {
         if(sub.tag() == "as_constructor") add_constructor(std::make_shared<as_constructor>(sub));
         else if(sub.tag() == "as_member_function") add_member_function(std::make_shared<as_member_function>(sub));
      }
   }
}

as_class::as_class(const std::string& name, bool verified, std::shared_ptr<as_description> descr)
: as_doc(verified,descr)
, m_name(name)
{}

as_class::~as_class()
{}

void as_class::add_constructor(std::shared_ptr<as_constructor> constr)
{
   m_constr[constr->key()] = constr;
}

void as_class::add_member_function(std::shared_ptr<as_member_function> mfun)
{
   m_mem_funs[mfun->key()] = mfun;
}

cf_syslib::xml_node as_class::to_xml(cf_syslib::xml_node& xml_parent)
{
   cf_syslib::xml_node xml_this = xml_parent.add_child(as_typeid(this));
   as_doc::to_xml(xml_this);

   xml_this.add_property("name",m_name);
   if(m_base.length() > 0)xml_this.add_property("base",m_base);
   for(auto& p : m_constr) {
      auto& constr = p.second;
      constr->to_xml(xml_this);
   }
   for(auto& p : m_mem_funs) {
      auto& mfun = p.second;
      mfun->to_xml(xml_this);
   }

   return xml_this;
}


std::shared_ptr<as_constructor> as_class::lookup_constructor(const std::string& signature, bool verified)
{
   std::shared_ptr<as_constructor> constr;
   auto it = m_constr.find(as_member_function::key(signature));
   if(it != m_constr.end()) {
      constr = it->second;
      constr->set_verified(verified);
   }
   return constr;
}

std::shared_ptr<as_member_function>  as_class::lookup_member_function(const std::string& signature, bool verified)
{
   std::shared_ptr<as_member_function> mfun;
   auto it = m_mem_funs.find(as_member_function::key(signature));
   if(it != m_mem_funs.end()) {
      mfun = it->second;
      mfun->set_verified(verified);
   }
   return mfun;
}

void as_class::unverify()
{
   set_verified(false);
   for(auto& p : m_constr) {
      (p.second)->unverify();
   }
   for(auto& p : m_mem_funs) {
      (p.second)->unverify();
   }
}

void as_class::add_base_candidate(std::string base_type)
{
   // strip away possible @ at the end
   size_t ipos = base_type.find_first_of('@');
   if(ipos != std::string::npos) base_type =  base_type.substr(0,ipos);
   m_base_candidates.insert(base_type);
}

bool as_class::has_unique_base_type(const std::string& base_type)
{
   return ((m_base_candidates.size()==1) && (m_base_candidates.find(base_type) != m_base_candidates.end()) );
}


bool as_class::resolve_base_type(as_xml* factory)
{
   m_base = "";
   for(auto i=m_base_candidates.begin(); i!=m_base_candidates.end(); i++) {
      const std::string& base_type = *i;
      std::shared_ptr<as_class> base_candidate = factory->lookup_class(base_type,true);
      if(base_candidate.get()) {

         for(auto j=m_base_candidates.begin(); j!=m_base_candidates.end(); j++) {
            const std::string& base_type_other = *j;
            if(base_candidate->has_unique_base_type(base_type_other)) {
               m_base_candidates.erase(j);
               j=m_base_candidates.begin();
            }
         }
      }
      else {
         m_base_candidates.erase(i);
         i = m_base_candidates.begin();
      }
   }

   if(m_base_candidates.size()==1) m_base = *m_base_candidates.begin();

   return (m_base_candidates.size() < 2);
}


void as_class::write_header(as_xml* factory, std::ostream& hfile)
{
   if(m_export_filter.find(m_name) == m_export_filter.end()) {

      hfile << std::endl;
      if(auto descr = description())descr->write_header(hfile,0);
      hfile << "class " << m_name;
      if(m_base.length()>0) hfile << " : public " << m_base;
      hfile << " {" << std::endl;
      hfile << "public:" << std::endl;
      for(auto& p : m_constr) {
         auto& constr = p.second;
         constr->write_header(hfile);
      }
      for(auto& p : m_mem_funs) {
         auto& mfun = p.second;

         // check if this function was inherited.
         // only export functions that were not inherited
         if(!function_inherited(factory,mfun)) {
            mfun->write_header(hfile);
         }
      }

      hfile << "};" << std::endl;
   }
}


bool as_class::function_inherited(as_xml* factory, std::shared_ptr<as_member_function> mfun)
{
   // look up base class
   if(auto base_type = factory->lookup_class(m_base,true)) {
      if(auto base_member = base_type->lookup_member_function(mfun->signature(),true)) {
         return true;
      }
   }
   return false;
}

void as_class::add_xml_todo(as_xml* factory, size_t level)
{
   if(level==1) {
      auto descr = description();
      if(!descr.get()) {
         set_description(std::make_shared<as_description>("XML_TODO"));
      }
   }

   if(level > 1) {
      for(auto c: m_constr) (c.second)->add_xml_todo(factory,level);
      for(auto m: m_mem_funs){
         auto mfun = m.second;
         if(!function_inherited(factory,mfun)) {
            if(!mfun->is_filtered(mfun->name())) {
               mfun->add_xml_todo(factory,level);
            }
         }
      }
   }
}
