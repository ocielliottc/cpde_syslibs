#include "as_constructor.h"

as_constructor::as_constructor(const std::string& signature, bool verified,  std::shared_ptr<as_description> descr)
: as_doc(verified,descr)
, m_signature(signature)
{}

as_constructor::as_constructor(cf_syslib::xml_node& node)
: as_doc(node)
{
   m_signature = node.get_property("signature","as_error");
   for(auto i=node.begin(); i!=node.end(); i++) {
      cf_syslib::xml_node sub(i);
      if(!sub.is_attribute_node()) {
         if(sub.tag() == "as_parameter") m_params.push_back(std::make_shared<as_parameter>(sub));
      }
   }
}

as_constructor::~as_constructor()
{}

void as_constructor::add_parameter(std::shared_ptr<as_parameter> param)
{
   m_params.push_back(param);
}

std::string as_constructor::key(size_t istart, const std::string& signature)
{
   std::string key_sig;
   std::vector<std::string> tokens;
   as_description::tokenize(signature," ",tokens);

   // skip the 1st token, the return type
   for(size_t i=istart;i<tokens.size();i++) {
      key_sig+=tokens[i];
   }
   return key_sig;
}

std::string as_constructor::key() const
{
   return key(1,m_signature);
}


cf_syslib::xml_node as_constructor::to_xml(cf_syslib::xml_node& xml_parent)
{
   cf_syslib::xml_node xml_this = xml_parent.add_child(as_typeid(this));
   as_doc::to_xml(xml_this);

   xml_this.add_property("signature",m_signature);
   for(auto& par : m_params) {
      par->to_xml(xml_this);
   }

   return xml_this;
}

std::shared_ptr<as_parameter> as_constructor::lookup_parameter(size_t index)
{
   std::shared_ptr<as_parameter> par;
   if(index < m_params.size()) par = m_params[index];
   return par;
}

void as_constructor::unverify()
{
   set_verified(false);
   for(auto& par : m_params) {
      par->unverify();
   }
}
