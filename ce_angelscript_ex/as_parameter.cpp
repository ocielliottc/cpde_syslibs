#include "as_parameter.h"

as_parameter::as_parameter(const std::string& type, const std::string& name, const std::string& defval,  bool verified, std::shared_ptr<as_description> descr)
: as_doc(verified,descr)
, m_type(type)
, m_name(name)
, m_defval(defval)
{
}

as_parameter::as_parameter(cf_syslib::xml_node& node)
: as_doc(node)
{
   m_type = node.get_property("type","as_error");
   m_name = node.get_property("name","as_error");
   m_defval = node.get_property("default","");
}

as_parameter::~as_parameter()
{}

cf_syslib::xml_node as_parameter::to_xml(cf_syslib::xml_node& xml_parent)
{
   cf_syslib::xml_node xml_this = xml_parent.add_child(as_typeid(this));
   as_doc::to_xml(xml_this);

   xml_this.add_property("type",m_type);
   xml_this.add_property("name",m_name);
   if(m_defval.length()>0)xml_this.add_property("default",m_defval);

   return xml_this;
}


void as_parameter::unverify()
{
   set_verified(false);
}
