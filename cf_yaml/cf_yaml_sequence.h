#ifndef CF_YAML_SEQUENCE_H
#define CF_YAML_SEQUENCE_H

#include "cf_yaml_value.h"
#include <vector>

// sequence class for for yml values parsed with cf_yaml_handler or created programmatically
class CF_YAML_PUBLIC cf_yaml_sequence : public cf_yaml_value {
public:
   cf_yaml_sequence();
   virtual ~cf_yaml_sequence();

   // push additional value to back of sequence
   void push_back(cf_yaml_value_ptr ptr) { m_vec.push_back(ptr); }

   // retrieve value by index
   cf_yaml_value_ptr get(size_t index, bool throw_exception = true);

public:
   virtual cf_yaml_ostream& to_yaml(cf_yaml_ostream& out) const;
   virtual void debug_print(size_t indent, std::ostream& out);

protected:
   size_t assign(cf_yaml_value_ptr ptr);

private:
   std::vector<cf_yaml_value_ptr> m_vec;
};

#endif // CF_YAML_SEQUENCE_H
