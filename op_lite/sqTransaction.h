#ifndef SQTRANSACTION_H
#define SQTRANSACTION_H


#include "op_lite_config.h"

/*
** Author: Carsten Arnholm, August 2011
**
** This code follows the sqlite3 license model, i.e.:
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
*/

#include <string>
using namespace std;
struct sqlite3;

/// low level: SQL database transaction
class OP_LITE_PUBLIC sqTransaction {
public:

   /// begin transaction on datbase db
   sqTransaction(sqlite3* db);

   /// destructor, end of transaction
   virtual ~sqTransaction();

   /// starts a new transaction explicitly
   bool begin_transaction();

   /// ends current transaction explicitly
   bool end_transaction();

   /// return the sqlite handle. if null, the transaction is dummy
   sqlite3* dbh();

private:
   string          m_name;    // transaction name
   sqlite3*        m_dbh;     // Sqlite database handle
private:
   static size_t   m_level;   // transaction nesting level
};

#endif // SQTRANSACTION_H
