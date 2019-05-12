#include "as_line.h"
#include "as_typeid.h"

as_line::as_line(const std::string& text)
: m_text(text)
{}

as_line::~as_line()
{}

as_line::as_line(cf_syslib::xml_node& node)
: m_text(node.get_value(""))
{}

cf_syslib::xml_node as_line::to_xml(cf_syslib::xml_node& xml_parent)
{
   cf_syslib::xml_node xml_this = xml_parent.add_child(as_typeid(this));
   xml_this.put_value(m_text);
   return xml_this;
}
