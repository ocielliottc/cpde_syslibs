#include "as_doc.h"

as_doc::as_doc(bool verified, std::shared_ptr<as_description> descr)
: m_verified(verified)
, m_descr(descr)
{}

as_doc::as_doc(cf_syslib::xml_node& node)
: m_verified(node.get_property("verified",false))
{
   cf_syslib::xml_node child;
   if(node.get_child("description",child)) {
      m_descr = std::make_shared<as_description>(child);
   }
}

as_doc::~as_doc()
{}

cf_syslib::xml_node as_doc::to_xml(cf_syslib::xml_node& xml_parent)
{
   xml_parent.add_property("verified",m_verified);
   if(m_descr.get())m_descr->to_xml(xml_parent);
   return xml_parent;
}
