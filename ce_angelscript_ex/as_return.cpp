#include "as_return.h"

as_return::as_return(cf_syslib::xml_node& node)
: as_doc(node)
, m_type(node.get_property("type","as_error"))
{}

as_return::as_return(const std::string& type,  bool verified, std::shared_ptr<as_description> descr)
: as_doc(verified, descr)
, m_type(type)
{
}

as_return::~as_return()
{}


cf_syslib::xml_node as_return::to_xml(cf_syslib::xml_node& xml_parent)
{
   cf_syslib::xml_node xml_this = xml_parent.add_child(as_typeid(this));
   xml_this.add_property("type",m_type);
   as_doc::to_xml(xml_this);

   return xml_this;
}

void as_return::unverify()
{
   set_verified(false);
}


std::string as_return::doxy_string()
{
   if(m_type == "void")return "";
   std::string txt = " \\return " + m_type;
   if(auto descr = description()) txt += ", " + descr->line(0).text();
   return txt;
}
