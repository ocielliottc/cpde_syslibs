#include "cf_yaml_ostream.h"
#include "cf_yaml.h"

cf_yaml_ostream::cf_yaml_ostream()
: m_out(std::make_shared<YAML::Emitter>())
{}

cf_yaml_ostream::cf_yaml_ostream(std::ostream& stream)
: m_out(std::make_shared<YAML::Emitter>(stream))
{}


cf_yaml_ostream::~cf_yaml_ostream()
{}

YAML::Emitter& cf_yaml_ostream::emitter()
{
   return *m_out;
}
