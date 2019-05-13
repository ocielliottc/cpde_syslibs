#ifndef AS_CONSTRUCTOR_H
#define AS_CONSTRUCTOR_H
#include "as_doc.h"
#include "as_parameter.h"

// as_constructor is a constructor function of a class
class as_constructor : public as_doc {
public:
   as_constructor(cf_syslib::xml_node& node);
   as_constructor(const std::string& signature, bool verified,  std::shared_ptr<as_description> descr = 0);
   virtual ~as_constructor();

   // unverify this constructor and all descendants
   void unverify();

   // return raw signature as defined by asIScriptFunction::GetDeclaration
   std::string  signature() const { return m_signature; }

   // find a parameter by index (may return 0)
   std::shared_ptr<as_parameter> lookup_parameter(size_t index);

   // key returns the signature string, but with return type and all white space removed
   std::string key() const;

   // return number of parameters
   size_t size() const { return m_params.size(); }

   // compute key from signature
   static std::string key(size_t istart, const std::string& signature);

   // add constructor parameter
   void add_parameter(std::shared_ptr<as_parameter> param);

   cf_syslib::xml_node to_xml(cf_syslib::xml_node& xml_parent);

private:
   std::string                                m_signature; // angelscript signature as returned by asIScriptFunction::GetDeclaration
   std::vector<std::shared_ptr<as_parameter>> m_params;
};

#endif // AS_CONSTRUCTOR_H
