#include "cf_yaml.h"

#include "cf_yaml_doc.h"
#include "cf_yaml_handler.h"


cf_yaml_doc::cf_yaml_doc()
{
   //ctor
}

cf_yaml_doc::~cf_yaml_doc()
{
   //dtor
}

cf_yaml_doc_ptr cf_yaml_doc::parse(std::istream& in)
{
   cf_yaml_doc_ptr result;

   YAML::Parser parser(in);
   cf_yaml_handler handler;
   if(parser.HandleNextDocument(handler)) {
      result = handler.document();
   }


   return result;
}

cf_yaml_value_ptr cf_yaml_doc::assign_root(cf_yaml_value_ptr root)
{
   m_root = root;
   return m_root;
}


cf_yaml_ostream&  cf_yaml_doc::to_yaml(cf_yaml_ostream& out) const
{
   if(m_root.get()) {
      m_root->to_yaml(out);
   }
   return out;
}
