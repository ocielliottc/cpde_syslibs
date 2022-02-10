#ifndef CF_YAML_VALUE_H
#define CF_YAML_VALUE_H

#include <string>
#include <memory>
#include <ostream>
#include "cf_yaml_config.h"

class cf_yaml_value;
using cf_yaml_value_ptr = std::shared_ptr<cf_yaml_value>;

// abstract base class for yml values parsed with cf_yaml_handler
class CF_YAML_PUBLIC cf_yaml_value {
public:
   cf_yaml_value();
   virtual ~cf_yaml_value() = 0;

   virtual std::string value() const { return ""; }
   virtual size_t assign(cf_yaml_value_ptr ptr) { return 0; }
   virtual bool is_scalar() const { return false; }

   // retrieve value by string key (typical for map)
   virtual cf_yaml_value_ptr get(const std::string& key, bool throw_exception = true);

   // retrieve value by index (typical for sequence)
   virtual cf_yaml_value_ptr get(size_t index, bool throw_exception = true);

   virtual void debug_print(size_t indent, std::ostream& out) = 0;
};

#endif // CF_YAML_VALUE_H
