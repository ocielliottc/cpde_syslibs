#include "as_class.h"
#include "as_constructor.h"
#include "as_member_function.h"

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
