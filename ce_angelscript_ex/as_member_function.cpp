#include "as_member_function.h"
#include "as_parameter.h"
#include "as_return.h"

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
