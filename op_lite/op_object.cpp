/*
** Author: Carsten Arnholm, November 2014
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

#include "op_object.h"
#include "op_class_name.h"
#include "sqTable.h"
#include "sqDatabase.h"

op_object::op_object()
{}

op_object::~op_object()
{
   // make sure this object is removed from the persistent object cache
   if(sqTable* table = m_pid.table()) {
      op_database* db = table->db();
      db->erase_cached(this);
   }
}

bool op_object::update_persistent()
{
   if(op_database* db = this->db()) {
      return db->update_persistent(this);
   }
   return false;
}

string op_object::op_type() const
{
   string raw_name = typeid(*this).name();
   string name = op_class_name(raw_name);
   return name;
}

op_database* op_object::db() const
{
   if(sqTable* table = m_pid.table()) {
      return table->db();
   }
   return 0;
}

const op_pid& op_object::pid() const
{
   return m_pid;
}

void op_object::bind_persistent(const op_pid& refid)
{
   m_pid  = refid;
}

bool op_object::is_persistent() const
{
   if(sqTable* table = m_pid.table()) {
      IDint64 id = m_pid.id();
      return (id > 0);
   }
   return false;
}
