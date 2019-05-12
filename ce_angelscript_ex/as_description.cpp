#include "as_description.h"
#include "ce_angelscript_ex/as_typeid.h"
#include "as_doc.h"

as_description::as_description(cf_syslib::xml_node& node)
{
   for(auto i=node.begin(); i!=node.end(); i++) {
      cf_syslib::xml_node sub(i);
      if(!sub.is_attribute_node()) {
         if(sub.tag() == "as_line") m_lines.push_back(sub);
      }
   }
}

as_description::as_description(const std::string& text)
{
   if(text.length() > 0) {
      std::vector<std::string> tokens;
      tokenize(text,"\r\n",tokens);
      for(auto& line : tokens) {
         m_lines.push_back(line);
      }
   }
}

as_description::as_description(const as_line& line)
{
   m_lines.push_back(line);
}

as_description::~as_description()
{}

void as_description::add_line(const as_line& line)
{
   m_lines.push_back(line);
}

cf_syslib::xml_node as_description::to_xml(cf_syslib::xml_node& xml_parent)
{
   cf_syslib::xml_node xml_this = xml_parent.add_child(as_typeid(this));
   for(auto& line : m_lines) {
      line.to_xml(xml_this);
   }
   return xml_this;
}


void as_description::tokenize(const std::string& input,
                      const std::string& delimiters,
                      std::vector<std::string>& tokens)
{
   using namespace std;
   string::size_type last_pos = 0;
   string::size_type pos = 0;
   while(true) {
      pos = input.find_first_of(delimiters, last_pos);
      if( pos == string::npos ) {
         if(input.length()-last_pos > 0)tokens.push_back(input.substr(last_pos));
         break;
      }
      else {
         if(pos-last_pos > 0)tokens.push_back(input.substr(last_pos, pos - last_pos));
         last_pos = pos + 1;
      }
   }
}
