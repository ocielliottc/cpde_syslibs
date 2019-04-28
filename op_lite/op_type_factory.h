#ifndef OP_TYPE_FACTORY_H
#define OP_TYPE_FACTORY_H

#include "op_class_factory.h"
#include <unordered_map>
using namespace std;

/// The op_type_factory manages all persistent class factories, allowing for polymorphic persistent objects
class OP_LITE_PUBLIC op_type_factory {
   friend class op_manager;
public:
   typedef unordered_map<string,op_class_factory*> FactoryMap;
   virtual ~op_type_factory();

   /// install a new class factory (i.e. use op_persistent_class<T>() )
   void install(op_class_factory* factory);

   /// construct a *transient* object of given type using its default constructor
   /*! \param op_type string equivalent to op_class_name(typeid(T).name()), where T is derivative of op_object */
   op_object* default_construct(const string& op_type);

   /// return true if type is installed
   /*! \param op_type string equivalent to op_class_name(typeid(T).name()), where T is derivative of op_object */
   bool type_installed(const string& op_type) const;

   /// erase all class factories
   void clear();

protected:
   /// \privatesection (hidden in documentation)

   op_type_factory();

   /// alternative type install, where type is not necessarily the same as class name (use with care)
   void install(const string& type, op_class_factory* factory);

private:
   FactoryMap  m_factories;
};

#endif // OP_TYPE_FACTORY_H
