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

#include "sqTransaction.h"
#include "sqDatabase.h"
#include "sqlite3/sqlite3.h"
#include <stdexcept>
#include <sstream>

#include <string>
using namespace std;

size_t   sqTransaction::m_level = 0;

sqTransaction::sqTransaction(sqlite3* dbh)
: m_dbh(dbh)
{
   begin_transaction();
}

sqTransaction::~sqTransaction()
{
   end_transaction();
}

bool sqTransaction::begin_transaction()
{
   // do nothing if the database is not defined
   if(!m_dbh)return true;

   ostringstream out;
   out << "SPTRANS" << m_level+1;
   m_name = out.str();
   string sql = "SAVEPOINT " + m_name;

   if(SQLITE_OK == sqlite3_exec(m_dbh, sql.c_str(), 0, 0, 0)) {
      m_level++;
      return true;
   }
   else {
      // something went wrong, throw exception w/message
      string message = string("FATAL (sqTransaction::begin_transaction): ") + sqlite3_errmsg(m_dbh);
      throw runtime_error(message);
   }
   return false;
}

bool sqTransaction::end_transaction()
{
   if(!m_dbh)return true;

   string sql = "RELEASE " + m_name;

   // RELEASE ends the transaction
   if(SQLITE_OK == sqlite3_exec(m_dbh, sql.c_str(), 0, 0, 0)) {
      m_level--;
      return true;
   }
   else {
      // something went wrong, throw exception w/message
      string message = string("FATAL (sqTransaction::end_transaction): ") + sqlite3_errmsg(m_dbh);
      throw runtime_error(message);
   }
   return false;

}
sqlite3* sqTransaction::dbh()
{
   return m_dbh;
}
