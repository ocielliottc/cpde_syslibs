#ifndef AS_CLASS_H
#define AS_CLASS_H

#include "as_doc.h"
#include <map>
class as_constructor;
class as_member_function;

// as_class represents a complete scripting class
class as_class : public as_doc {
public:
   as_class(cf_syslib::xml_node& node);
   as_class(const std::string& name, bool verified, std::shared_ptr<as_description> descr);
   virtual ~as_class();

   // look up constructor by type signature (may return null)
   std::shared_ptr<as_constructor>     lookup_constructor(const std::string& signature, bool verified);
   std::shared_ptr<as_member_function> lookup_member_function(const std::string& signature, bool verified);

   // set name of base class
   void set_base(const std::string& base) { m_base = base; }

   std::string name() const { return m_name; }
   std::string base() const { return m_base; }

   void add_constructor(std::shared_ptr<as_constructor> constr);
   void add_member_function(std::shared_ptr<as_member_function> mfun);

   cf_syslib::xml_node to_xml(cf_syslib::xml_node& xml_parent);

   void unverify();

private:
   std::string m_name;  // name of class
   std::string m_base;  // name of base class or empty string if none

   std::map<std::string,std::shared_ptr<as_constructor>>     m_constr;     // key = as_constructor::key()
   std::map<std::string,std::shared_ptr<as_member_function>> m_mem_funs;   // key = as_member_function::key()
};

#endif // AS_CLASS_H
