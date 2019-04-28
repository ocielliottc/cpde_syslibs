#ifndef OP_CLASS_NAME_H
#define OP_CLASS_NAME_H

#include "op_lite_config.h"
#include <string>
#include <typeinfo>

/// return persistent class name as string
/*! \param typeid_name result of typeid(T).name(), where T is persistent class*/
OP_LITE_PUBLIC std::string op_class_name(const std::string& typeid_name);

/// return op_lite's persistent typeid string for any given class (template parameter)
template<class T>
std::string op_typeid() { return op_class_name(typeid(T).name()); }

#endif // OP_CLASS_NAME_H
