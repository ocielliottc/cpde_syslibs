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

#include "op_database.h"
#include "op_object.h"
#include "op_transaction.h"
#include "sqRow.h"

#include <sstream>
using namespace std;

op_database::op_database()
{}

op_database::~op_database()
{
   flush_cached(true,true);
}

op_object* op_database::get_cached(const string& key)
{
   auto i = m_objects.left.find(key);
   if(i!=m_objects.left.end())return i->second;
   return 0;
}

bool op_database::insert_cached(const string& key, op_object* object)
{
   if(m_tlist.size() > 0) {
      // this object is being inserted into cache, update the current transaction object
      op_transaction* trans = m_tlist.front();
      trans->insert_cached(object);
   }
   return m_objects.left.insert(make_pair(key,object)).second;
}

void op_database::erase_cached(op_object* object)
{
   if(m_tlist.size() > 0) {
      // this object is being erased from cache, update the current transaction object
      op_transaction* trans = m_tlist.front();
      trans->erase_cached(object);
   }
   m_objects.right.erase(object);
}

void  op_database::flush_cached(bool update_database, bool delete_transient)
{
   auto objects = m_objects;

   for(auto i=objects.left.begin(); i!= objects.left.end(); i++) {
      op_object* object = i->second;
      if(update_database) {
         update_persistent(object);
      }
      if(delete_transient) {
         // delete will cause erase_cached to be called
         delete object;
      }
   }
}

void op_database::push_transaction(op_transaction* trans)
{
   m_tlist.push_front(trans);
}

void op_database::pop_transaction()
{
   m_tlist.pop_front();
}

bool op_database::object_exists(const string& ptype, IDint64 pid  )
{
   ostringstream query;
   list<sqRef> ids;
   query << " WHERE ROWID=" << pid;
   if(select_ids(ids,ptype,query.str())){
      return (ids.size()==1);
   }
   return false;
}
