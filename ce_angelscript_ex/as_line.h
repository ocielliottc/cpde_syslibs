#ifndef AS_LINE_H
#define AS_LINE_H

#include "cf_syslib/xml_node.h"

// as_line is a single line text, usually part of as_description
class as_line {
public:
   as_line(cf_syslib::xml_node& node);
   as_line(const std::string& text);
   virtual ~as_line();

   cf_syslib::xml_node to_xml(cf_syslib::xml_node& xml_parent);

   const std::string& text() const { return m_text; }

private:
   std::string m_text;
};

#endif // AS_LINE_H
