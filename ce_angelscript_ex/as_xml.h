#ifndef AS_XML_H
#define AS_XML_H

#include <map>
#include <string>
#include "as_class.h"
#include <ostream>
class asIScriptEngine;
class asITypeInfo;
class asIScriptFunction;
class as_class;

class as_xml {
public:
   as_xml(const std::string& appnam);
   virtual ~as_xml();

   // unverify all contents
   void unverify();

   // fill data from script engine
   void from_script_engine(asIScriptEngine* engine);


   // look up a class by type name (may return null)
   std::shared_ptr<as_class> lookup_class(const std::string& type_name, bool verified);

   // insert a class
   void insert(std::shared_ptr<as_class> type) { m_types[type->name()] = type; }

   // write to xml stream
   void write_xml(std::ostream& xml);

   // read from xml stream
   void read_xml(std::istream& xml);

protected:

   // add constructors to the class instance
   void from_script_engine_constructors(asIScriptEngine* engine, asITypeInfo* type, std::shared_ptr<as_class> class_type);
   void from_script_engine_parameters_constr(asIScriptEngine* engine, asIScriptFunction* func, std::shared_ptr<as_constructor> as_constr);

   // add member functions to the class instance
   void from_script_engine_member_functions(asIScriptEngine* engine, asITypeInfo* type, std::shared_ptr<as_class> class_type);
   void from_script_engine_parameters(asIScriptEngine* engine, asIScriptFunction* func, std::shared_ptr<as_member_function> as_mem_fun);

private:
   std::string                                     m_appnam; // name of application
   std::map<std::string,std::shared_ptr<as_class>> m_types;  // using a map, we get alphabetic sorting
};

#endif // AS_XML_H
