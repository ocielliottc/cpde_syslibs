#include "op_class_factory.h"

op_class_factory::op_class_factory()  {}
op_class_factory::~op_class_factory() {}


string op_class_factory::op_type() const
{
   op_object* object = this->construct();
   string type=object->op_type();
   delete object;
   return type;
}
