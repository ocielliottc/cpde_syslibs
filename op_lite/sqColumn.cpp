/*
** Author: Carsten Arnholm, January 2010
** This code was written for my weather station project
**
** This code follows the sqlite3 license model, i.e.:
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
*/

#include "sqColumn.h"

sqColumn::StringSet sqColumn::m_names;

sqColumn::string_i sqColumn::name_ref(const string& name)
{
   auto i = m_names.find(name);
   if(i == m_names.end()) {
      i = m_names.insert(name).first;
   }
   return i;
}

sqColumn::sqColumn()
: m_name(name_ref("Error"))
, m_pk(sqPK_NONE)
{}

sqColumn::sqColumn(const string& name, sqPK pk)
: m_name(name_ref(name))
, m_pk(pk)
{}

sqColumn::~sqColumn()
{}

const string& sqColumn::colnam() const
{
   return *m_name;
}

sqPK sqColumn::pk() const
{
   return m_pk;
}

bool sqColumn::is_pointer_type() const
{
   return (m_name->find(".POINTER") != string::npos);
}

bool sqColumn::is_compatible(const sqColumn* other) const
{
   if(m_name               != other->m_name)return false;
   if(this->sqlite3_type() != other->sqlite3_type())return false;
   if(this->pk()           != other->pk())return false;
   return true;
}
