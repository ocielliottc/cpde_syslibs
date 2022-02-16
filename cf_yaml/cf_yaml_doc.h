#ifndef CF_YAML_DOC_H
#define CF_YAML_DOC_H

#include <string>
#include <memory>
#include <istream>
#include "cf_yaml_value.h"
#include "cf_yaml_ostream.h"

class cf_yaml_doc;
using cf_yaml_doc_ptr = std::shared_ptr<cf_yaml_doc>;

// cf_yaml_doc is returned from cf_yaml_doc::parse() or created programmatically
class CF_YAML_PUBLIC cf_yaml_doc {
public:
   friend class cf_yaml_handler;

   // create document by parsing YAML text
   static cf_yaml_doc_ptr parse(std::istream& in);

   // create document by default constructor and root assignment
   cf_yaml_doc();
   cf_yaml_value_ptr assign_root(cf_yaml_value_ptr root);

   virtual ~cf_yaml_doc();

   bool is_empty() const { return (m_root==nullptr); }

   // return the root pointer
   cf_yaml_value_ptr get_root() const { return m_root; }

   cf_yaml_ostream& to_yaml(cf_yaml_ostream& out) const;

private:
   cf_yaml_value_ptr m_root;
};

inline cf_yaml_ostream& operator<<(cf_yaml_ostream& out, const cf_yaml_doc& doc)
{ return doc.to_yaml(out); }

inline cf_yaml_ostream& operator<<(cf_yaml_ostream& out, cf_yaml_doc_ptr doc)
{ return doc->to_yaml(out); }

#endif // CF_YAML_DOC_H
