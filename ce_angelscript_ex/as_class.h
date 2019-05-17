#ifndef AS_CLASS_H
#define AS_CLASS_H

#include "as_doc.h"
#include <map>
#include <set>
class as_constructor;
class as_member_function;
class as_xml;

// as_class represents a complete scripting class
class as_class : public as_doc {
public:
   as_class(cf_syslib::xml_node& node);
   as_class(const std::string& name, bool verified, std::shared_ptr<as_description> descr);
   virtual ~as_class();

   // unverify this class and all descendants
   void unverify();

   // look up constructor by type signature (may return null). Do not include return type in constructor signature
   std::shared_ptr<as_constructor>     lookup_constructor(const std::string& signature, bool verified);

   // look up member function by type signature (may return null). Do include return type in signature
   std::shared_ptr<as_member_function> lookup_member_function(const std::string& signature, bool verified);

   // set name of base class
   void set_base(const std::string& base) { m_base = base; }

   std::string name() const { return m_name; }
   std::string base() const { return m_base; }

   void add_constructor(std::shared_ptr<as_constructor> constr);
   void add_member_function(std::shared_ptr<as_member_function> mfun);

   cf_syslib::xml_node to_xml(cf_syslib::xml_node& xml_parent);

   const std::map<std::string,std::shared_ptr<as_constructor>>&  constr() const { return m_constr; }

   void add_base_candidate(std::string base_type);
   bool resolve_base_type(as_xml* factory);

   // return true if the function signature was inherited
   bool function_inherited(as_xml* factory, std::shared_ptr<as_member_function> mfun);

   void write_header(as_xml* factory, std::ostream& hfile);

   static void add_export_filter(const std::string& type_name) { m_export_filter.insert(type_name); }

   void add_xml_todo(as_xml* factory, size_t level);

protected:
   bool has_unique_base_type(const std::string& base_type);

private:
   std::string m_name;  // name of class
   std::string m_base;  // name of base class or empty string if none

   std::map<std::string,std::shared_ptr<as_constructor>>     m_constr;     // key = as_constructor::key()
   std::map<std::string,std::shared_ptr<as_member_function>> m_mem_funs;   // key = as_member_function::key()

   std::set<std::string> m_base_candidates; // only used temporarily during script engine processing
   static std::set<std::string> m_export_filter;  // names of classes that shall not be exported to header file
};

#endif // AS_CLASS_H
