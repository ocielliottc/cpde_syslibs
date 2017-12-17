#ifndef OP_CLASS_FACTORY_H
#define OP_CLASS_FACTORY_H

#include "op_object.h"
#include <string>
using namespace std;

/// The op_class_factory is an abstract interface for the op_persistent_class template class
class OP_LITE_PUBLIC op_class_factory {
public:
   op_class_factory();
   virtual ~op_class_factory();

   /// Construct a persistent object via its default constructor
   virtual op_object* construct() const = 0;

   /// return the type string of the persistent class managed by this factory
   virtual string op_type() const;
};

/// The op_persistent_class template is a generic class factory for declaring persistent types in the type factory
template <typename T>
class op_persistent_class : public op_class_factory {
public:
   /// Construct a persistent object of type T
   virtual T* construct() const { return new T(); }
};


#endif // OP_CLASS_FACTORY_H
