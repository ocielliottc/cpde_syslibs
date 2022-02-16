#ifndef CF_YAML_OSTREAM_H
#define CF_YAML_OSTREAM_H

#include "cf_yaml_config.h"
#include <memory>
#include <ostream>

namespace YAML  { class Emitter; }

// output stream for writing YAML text
class CF_YAML_PUBLIC cf_yaml_ostream {
public:
   cf_yaml_ostream();
   cf_yaml_ostream(std::ostream& stream);
   virtual ~cf_yaml_ostream();

   YAML::Emitter& emitter();

private:
   std::shared_ptr<YAML::Emitter> m_out;
};

#endif // CF_YAML_OSTREAM_H
