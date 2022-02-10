#ifndef CF_YAML_HANDLER_H
#define CF_YAML_HANDLER_H

#include "cf_yaml.h"

#include <list>
#include <vector>
#include "cf_yaml_doc.h"
#include "cf_yaml_value.h"

//
//      std::ifstream fin(ymlfile);
//      YAML::Parser parser(fin);
//      cf_yaml_handler handler;
//      if(parser.HandleNextDocument(handler)) {
//         cf_yaml_doc_ptr doc = handler.document();
//      }
//

// handler for parsing YML into cf_yaml_doc
class CF_YAML_PUBLIC cf_yaml_handler : public YAML::EventHandler {
public:
   using Mark = YAML::Mark;
   using anchor_t = YAML::anchor_t;

   cf_yaml_handler();
   virtual ~cf_yaml_handler();

   virtual void OnDocumentStart(const Mark& mark);
   virtual void OnDocumentEnd();
   virtual void OnNull(const Mark& mark, anchor_t anchor);
   virtual void OnAlias(const Mark& mark, anchor_t anchor);
   virtual void OnScalar(const Mark& mark, const std::string& tag, anchor_t anchor, const std::string& value);
   virtual void OnSequenceStart(const Mark& mark, const std::string& tag, anchor_t anchor, YAML::EmitterStyle::value style);
   virtual void OnSequenceEnd();

   virtual void OnMapStart(const Mark& mark, const std::string& tag, anchor_t anchor, YAML::EmitterStyle::value style);
   virtual void OnMapEnd();
   virtual void OnAnchor(const Mark& mark, const std::string& anhor_name );

   cf_yaml_doc_ptr document() { return m_doc; }

private:
   cf_yaml_doc_ptr              m_doc;
   std::list<cf_yaml_value_ptr> m_stack;
   std::vector<std::string>     m_anchors;
};

#endif // CF_YAML_HANDLER_H
