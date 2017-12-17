#ifndef SQSTATEMENT_H
#define SQSTATEMENT_H

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

#include "sqlite3/sqlite3.h"
#include <string>
using namespace std;

#include "sqRef.h"
class sqDatabase;
class sqRow;


/// sqQueryType enum, basic query types supported
enum sqQueryType {
   /// sqRECORD_INSERT insert new record
   sqRECORD_INSERT,
   /// sqRECORD_UPDATE update existing record
   sqRECORD_UPDATE,
   /// sqRECORD_DELETE delete existing record
   sqRECORD_DELETE
};


/// low level: sqStatement  represents low level sql executable statement, i.e. INSERT, UPDATE, DELETE, used from sqQuery only
class sqStatement {
   friend class sqQuery;

protected:
   /// \privatesection (hidden in documentation)

   /// constructor
   sqStatement(sqDatabase* db, const sqRef& ref, const sqRow& object, sqQueryType qtype);

   /// destructor
   virtual ~sqStatement();

   /// check if query succeeded
   bool ok();

   /// message if something was wrong
   string message();

private:
   bool execute_statement(const sqRow& object);

private:
   /// parameters from constructor, not owned here
   sqDatabase*   m_db;
   sqRef         m_ref;
   sqQueryType   m_qtype;

   /// parameters owned here
   sqlite3_stmt* m_stmt;
   string        m_query;
   string        m_message;
   bool          m_ok;
};

#endif // SQSTATEMENT_H
