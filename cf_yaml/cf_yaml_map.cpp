#include "cf_yaml_map.h"
#include <stdexcept>
#include "cf_yaml.h"

cf_yaml_map::cf_yaml_map()
{}

cf_yaml_map::~cf_yaml_map()
{}

void cf_yaml_map::insert(const std::string& key, cf_yaml_value_ptr value)
{
   m_map[key] = value;
   m_insert_order.push_back(key);
}

size_t cf_yaml_map::assign(cf_yaml_value_ptr ptr)
{
   m_cache.push_back(ptr);

   while(m_cache.size() > 1) {

      cf_yaml_value_ptr val = m_cache.back();
      m_cache.pop_back();

      cf_yaml_value_ptr key = m_cache.back();
      m_cache.pop_back();

      std::string key_string = key->value();
      if(key_string.length() > 0) {
         insert(key_string,val);
      }
   }

   return 1;
}

cf_yaml_value_ptr cf_yaml_map::get(const std::string& key, bool throw_exception)
{
   auto it = m_map.find(key);
   if(it != m_map.end()) return it->second;

   if(!throw_exception) return nullptr;
   throw std::logic_error("Key '" + key + "' does not exist for " + typeid(*this).name());

}

void cf_yaml_map::debug_print(size_t indent, std::ostream& out)
{
   std::string ind,ind3;
   ind.resize(indent,' ');
   ind3.resize(indent+3,' ');
   out << std::endl << ind << "<m"<< m_map.size() <<"> {";

   for(auto key : m_insert_order) {
      auto v = m_map[key];
      out << std::endl << ind3 << key << ": ";
      if(v->is_scalar()) {
         out << ' ' << v->value();
      }
      else {
         v->debug_print(indent+6,out);
      }
   }
   out << std::endl << ind <<" }";
}

cf_yaml_ostream&  cf_yaml_map::to_yaml(cf_yaml_ostream& out) const
{
   auto& yout = out.emitter();

   yout << YAML::BeginMap;
   for(auto key : m_insert_order) {
      auto it = m_map.find(key);
      if(it != m_map.end()) {
         yout << YAML::Key << key;
         yout << YAML::Value;
         it->second->to_yaml(out);
      }
   }
   yout << YAML::EndMap;

   return out;
}
