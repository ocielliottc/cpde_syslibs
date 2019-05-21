// BeginLicense:
// Part of: cpde_syslibs - Cross Platform Development Environment, system libraries
// Copyright (C) 2017 Carsten Arnholm
// All rights reserved
//
// Permission is granted to anyone to use this software for any
// purpose, including commercial applications, and to alter it and
// redistribute it freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you
//    must not claim that you wrote the original software. If you use
//    this software in a product, an acknowledgment in the product
//    documentation would be appreciated.
//
// 2. Altered source versions must be plainly marked as such, and
//    must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
// This file is provided "AS IS" with NO WARRANTY OF ANY KIND,
// INCLUDING THE WARRANTIES OF DESIGN, MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE.
// EndLicense:

#include "as_doc.h"

as_doc::as_doc(bool verified, std::shared_ptr<as_description> descr)
: m_verified(verified)
, m_descr(descr)
{}

as_doc::as_doc(cf_syslib::xml_node& node)
: m_verified((node.get_property("verified",0))? true : false)
{
   cf_syslib::xml_node child;
   if(node.get_child("as_description",child)) {
      m_descr = std::make_shared<as_description>(child);
   }
}

as_doc::~as_doc()
{}

cf_syslib::xml_node as_doc::to_xml(cf_syslib::xml_node& xml_parent)
{
   xml_parent.add_property("verified",((m_verified)? 1 : 0));
   if(m_descr.get())m_descr->to_xml(xml_parent);
   return xml_parent;
}
