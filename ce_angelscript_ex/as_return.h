#ifndef AS_RETURN_H
#define AS_RETURN_H

#include "as_doc.h"

// as_return is a return value from a function
class as_return : public as_doc {
public:
   as_return(cf_syslib::xml_node& node);
   as_return(const std::string& type,  bool verified, std::shared_ptr<as_description> descr = 0);
   virtual ~as_return();

   void set_type(const std::string& type) { m_type = type; }
   std::string type() const { return m_type; }

   cf_syslib::xml_node to_xml(cf_syslib::xml_node& xml_parent);

   void unverify();

private:
   std::string m_type;    // type name of parameter, e.g. "double"
};

#endif // AS_RETURN_H
