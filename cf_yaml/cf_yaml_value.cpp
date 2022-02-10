#include "cf_yaml_value.h"
#include <stdexcept>

cf_yaml_value::cf_yaml_value()
{}

cf_yaml_value::~cf_yaml_value()
{}

cf_yaml_value_ptr cf_yaml_value::get(const std::string& key, bool throw_exception)
{
   if(!throw_exception) return nullptr;
   throw std::logic_error("Cannot look up key '" + key + "' for " + typeid(*this).name());
}

cf_yaml_value_ptr cf_yaml_value::get(size_t index, bool throw_exception)
{
   if(!throw_exception) return nullptr;
   throw std::logic_error("Cannot look up index '" + std::to_string(index) + "' for " + typeid(*this).name());
}
