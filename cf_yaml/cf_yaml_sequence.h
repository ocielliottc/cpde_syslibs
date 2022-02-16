#ifndef CF_YAML_SEQUENCE_H
#define CF_YAML_SEQUENCE_H

#include "cf_yaml_value.h"
#include <vector>

// sequence class for yml values parsed with cf_yaml_handler
class CF_YAML_PUBLIC cf_yaml_sequence : public cf_yaml_value {
public:
   cf_yaml_sequence();
   virtual ~cf_yaml_sequence();

   size_t assign(cf_yaml_value_ptr ptr);

   // retrieve value by index
   cf_yaml_value_ptr get(size_t index, bool throw_exception = true);

   virtual cf_yaml_ostream& to_yaml(cf_yaml_ostream& out) const;

   void debug_print(size_t indent, std::ostream& out);
private:
   std::vector<cf_yaml_value_ptr> m_vec;
};

#endif // CF_YAML_SEQUENCE_H
