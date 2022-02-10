#include "cf_yaml_scalar.h"

cf_yaml_scalar::cf_yaml_scalar(const std::string& value)
: m_value(value)
{}

cf_yaml_scalar::~cf_yaml_scalar()
{}

void cf_yaml_scalar::debug_print(size_t indent, std::ostream& out)
{
   std::string ind;
   ind.resize(indent,' ');
   out << ind << m_value;
}
