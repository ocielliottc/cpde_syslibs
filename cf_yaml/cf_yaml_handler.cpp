#include "cf_yaml_handler.h"
#include "cf_yaml_scalar.h"
#include "cf_yaml_sequence.h"
#include "cf_yaml_map.h"

#include <iostream>

cf_yaml_handler::cf_yaml_handler()
{}

cf_yaml_handler::~cf_yaml_handler()
{}

void cf_yaml_handler::OnDocumentStart(const Mark& mark)
{
   // create new document and clear stack + anchors
   m_doc = std::make_shared<cf_yaml_doc>();
   m_stack.clear();
   m_anchors.clear();
}

void cf_yaml_handler::OnDocumentEnd()
{}

void cf_yaml_handler::OnNull(const Mark& mark, anchor_t anchor)
{
   // for now we treat NULL objects as scalars with a tilde string
   auto scalar = std::make_shared<cf_yaml_scalar>("~");

   if(m_stack.size() > 0) {
      // assign scalar to current object
      cf_yaml_value_ptr current = m_stack.back();
      current->assign(scalar);
   }
}

void cf_yaml_handler::OnAlias(const Mark& mark, anchor_t anchor)
{
}

void cf_yaml_handler::OnScalar(const Mark& mark, const std::string& tag, anchor_t anchor, const std::string& value)
{
   auto scalar = std::make_shared<cf_yaml_scalar>(value);

   if(m_stack.size() > 0) {
      // assign scalar to current object
      cf_yaml_value_ptr current = m_stack.back();
      current->assign(scalar);
   }
}

void cf_yaml_handler::OnSequenceStart(const Mark& mark, const std::string& tag, anchor_t anchor, YAML::EmitterStyle::value style)
{
   // create new sequence and make it a root object unless a root already exists
   auto sequence = std::make_shared<cf_yaml_sequence>();
   if(m_doc->is_empty()) m_doc->assign_root(sequence);

   if(m_stack.size() > 0) {
      // assign sequence to current object
      cf_yaml_value_ptr current = m_stack.back();
      current->assign(sequence);
   }

   // make sequence the current object
   m_stack.push_back(sequence);
}

void cf_yaml_handler::OnSequenceEnd()
{
   // pop sequence from stack
   m_stack.pop_back();
}


void cf_yaml_handler::OnMapStart(const Mark& mark, const std::string& tag, anchor_t anchor, YAML::EmitterStyle::value style)
{
   // create new map and make it a root object unless a root already exists
   auto ymap =std::make_shared<cf_yaml_map>();
   if(m_doc->is_empty()) m_doc->assign_root(ymap);

   if(m_stack.size() > 0) {
      // assign map to current object
      cf_yaml_value_ptr current = m_stack.back();
      current->assign(ymap);
   }

   // make map the current object
   m_stack.push_back(ymap);
}

void cf_yaml_handler::OnMapEnd()
{
   // pop map from stack
   m_stack.pop_back();
}

void cf_yaml_handler::OnAnchor(const Mark& mark, const std::string& anchor_name )
{
   // add anchor to vector of anchors
   // NOTE: Anchors are not fully implemented
   m_anchors.push_back(anchor_name);
}
