#ifndef OP_TRANSACTION_H
#define OP_TRANSACTION_H

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

#include <set>
using namespace std;

#include "op_lite_config.h"
#include "sqTransaction.h"
class op_database;
class op_object;

/// An op_transaction represents database and cached object transactions
class OP_LITE_PUBLIC op_transaction {
   friend class op_database;

public:
   /// begin transaction on currently selected database
   op_transaction(bool readonly = false);

   /// begin write transaction
   op_transaction(op_database* db);

   /// begin (optionally) read only transaction
   op_transaction(op_database* db, bool readonly);

   /// destructor ends transaction
   virtual ~op_transaction();

protected:
   /// \privatesection (hidden in documentation)

   // the database will report caching of objects
   void insert_cached(op_object* object);
   void erase_cached(op_object* object);

private:
   op_database*  m_db;
   sqTransaction m_sqtrans;

   // the transaction set contains the cached objects being created during the transaction
   typedef set<op_object*> TransactionSet;
   TransactionSet  m_objects;
};

#endif // OP_TRANSACTION_H
