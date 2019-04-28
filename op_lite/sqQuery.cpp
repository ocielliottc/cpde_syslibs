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

#include "sqQuery.h"
#include "sqDatabase.h"
#include "sqTable.h"
#include "sqStatement.h"

sqQuery::sqQuery(sqDatabase* db, const string& query)
: m_message("")
, m_ok(false)
{
   char* zErrMsg = 0;
   if(SQLITE_OK != sqlite3_exec(db->dbh(),query.c_str(), 0, 0, &zErrMsg)) {
      m_message = zErrMsg;
      sqlite3_free(zErrMsg);
   }
   else {
      m_ok = true;
   }
}

sqQuery::sqQuery(sqDatabase* db, sqTable* table, const sqRow& object, sqQueryType qtype)
: m_message("")
, m_ok(false)
{
   sqRef ref(table);
   sqStatement statement(db,ref,object,qtype);
   m_ok = statement.ok();
   if(!m_ok) {
      m_message = statement.message();
   }
}

sqQuery::sqQuery(sqDatabase* db, const sqRef& id, const sqRow& object, sqQueryType qtype)
: m_message("")
, m_ok(false)
{
   sqStatement statement(db,id,object,qtype);
   m_ok = statement.ok();
   if(!m_ok) {
      m_message = statement.message();
   }
}

sqQuery::~sqQuery()
{}

bool sqQuery::ok()
{
   return m_ok;
}

string sqQuery::message()
{
   return m_message;
}

sqRef sqQuery::row_id(sqTable* table)
{
   IDint64 id = sqlite3_last_insert_rowid(table->db()->dbh());
   return sqRef(table,id);
}
