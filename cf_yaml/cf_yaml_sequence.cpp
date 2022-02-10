#include "cf_yaml_sequence.h"
#include <stdexcept>

cf_yaml_sequence::cf_yaml_sequence()
{}

cf_yaml_sequence::~cf_yaml_sequence()
{}

size_t cf_yaml_sequence::assign(cf_yaml_value_ptr value)
{
   m_vec.push_back(value);
   return 1;
}

cf_yaml_value_ptr cf_yaml_sequence::get(size_t index, bool throw_exception)
{
   if(index < m_vec.size()) return m_vec[index];
   if(!throw_exception) return nullptr;
   throw std::logic_error("Cannot look up 0-based index '" + std::to_string(index) + "' for " + typeid(*this).name() + ", size=" + std::to_string(m_vec.size()) );
}

void cf_yaml_sequence::debug_print(size_t indent, std::ostream& out)
{
   std::string ind;
   ind.resize(indent,' ');
   size_t num_non_scalar=0;
   out << std::endl << ind<< "<s"<< m_vec.size()<<"> [";
   for(auto v : m_vec) {
      if(v->is_scalar()) {
         out << ' ' << v->value();
      }
      else {
         v->debug_print(indent+3,out);
         num_non_scalar++;
      }
   }
   if(num_non_scalar==0) out << " ]";
   else                  out << std::endl << ind <<" ]";
}

