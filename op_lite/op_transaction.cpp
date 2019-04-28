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
#include "op_manager.h"
#include "op_transaction.h"
#include "op_database.h"
#include "op_object.h"

op_transaction::op_transaction(bool readonly)
: m_db(op_mgr()->selected_database())
, m_sqtrans( (readonly)? 0 : op_mgr()->selected_database()->dbh() )
{
   m_db->push_transaction(this);
}

op_transaction::op_transaction(op_database* db)
: m_db(db)
, m_sqtrans(db->dbh())
{
   m_db->push_transaction(this);
}

op_transaction::op_transaction(op_database* db, bool readonly)
: m_db(db)
, m_sqtrans( (readonly)? 0 : db->dbh() )
{
   // register transaction in database
   m_db->push_transaction(this);
}

op_transaction::~op_transaction()
{
   while(m_objects.size() > 0) {
      op_object* object = *m_objects.begin();

      // update the object in the database unless this transaction is a read-only transaction
      if(m_sqtrans.dbh()) {
         object->update_persistent();
      }

      // delete will cause the object to be removed from cache, and therefore also from here
      delete object;
   }

   // make sure this transaction is forgotten by the database
   m_db->pop_transaction();
}

void op_transaction::insert_cached(op_object* object)
{
   m_objects.insert(object);
}

void op_transaction::erase_cached(op_object* object)
{
   m_objects.erase(object);
}
