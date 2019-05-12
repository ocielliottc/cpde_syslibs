#ifndef AS_PARAMETER_H
#define AS_PARAMETER_H

#include "as_doc.h"

// as_parameter is a parameter in a function call
class as_parameter : public as_doc {
public:
   as_parameter(cf_syslib::xml_node& node);
   as_parameter(const std::string& type, const std::string& name, const std::string& defval,  bool verified, std::shared_ptr<as_description> descr = 0);
   virtual ~as_parameter();

   void set_name(const std::string& name) { m_name = name; }
   void set_type(const std::string& type) { m_type = type; }
   void set_defval(const std::string& defval) { m_defval = defval; }

   std::string name() const { return m_name; }

   cf_syslib::xml_node to_xml(cf_syslib::xml_node& xml_parent);

   void unverify();

private:
   std::string m_type;    // type name of parameter, e.g. "double"
   std::string m_name;    // parameter name
   std::string m_defval;  // default value or empty string if no default
};

#endif // AS_PARAMETER_H
