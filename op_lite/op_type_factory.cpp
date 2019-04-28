#include "op_type_factory.h"
#include <stdexcept>
using namespace std;

op_type_factory::op_type_factory()
{}

op_type_factory::~op_type_factory()
{
   clear();
}

void op_type_factory::clear()
{
   for(auto i=m_factories.begin(); i!=m_factories.end(); i++) {
      op_class_factory* factory = i->second;
      delete factory;
   }
   m_factories.clear();
}

void op_type_factory::install(op_class_factory* factory)
{
   string type = factory->op_type();
   install(type,factory);
}


void op_type_factory::install(const string& type, op_class_factory* factory)
{
   auto i = m_factories.find(type);
   if(i == m_factories.end()) {
      m_factories.insert(make_pair(type,factory));
   }
   else {
      throw logic_error("op_type_factory::insert, type already registered: " + type);
   }
}

op_object* op_type_factory::default_construct(const string& op_type)
{
   auto i = m_factories.find(op_type);
   if(i != m_factories.end()) {
      op_class_factory* factory = i->second;
      return factory->construct();
   }
   throw logic_error("op_type_factory::default_construct, type not registered: " + op_type);
}

bool op_type_factory::type_installed(const string& op_type) const
{
   auto i = m_factories.find(op_type);
   return (i !=  m_factories.end());
}
