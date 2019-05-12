#ifndef AS_DOC_H
#define AS_DOC_H

#include <vector>
#include <string>
#include <memory>
#include "cf_syslib/xml_node.h"
#include "as_typeid.h"
#include "as_description.h"

// as_doc is the base class for several types, it manages description data
class as_doc {
public:
   as_doc(bool verified, std::shared_ptr<as_description> descr);
   as_doc(cf_syslib::xml_node& node);
   virtual ~as_doc();

   // provide read & write access to description
   std::shared_ptr<as_description> description() { return m_descr; }

   // set verification flag
   void set_verified(bool verified = true) { m_verified = verified; }

   // inverify this object and all descendants
   virtual void unverify() = 0;

   // set new description
   void set_description(std::shared_ptr<as_description> descr) { m_descr = descr; }

   // export this object to XML DOM tree, under xml_parent
   virtual cf_syslib::xml_node to_xml(cf_syslib::xml_node& xml_parent) = 0;

private:
   bool                            m_verified;  // true when verified by script engine
   std::shared_ptr<as_description> m_descr;
};

#endif // AS_DOC_H