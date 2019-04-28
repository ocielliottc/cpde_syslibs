#ifndef SQQUERY_H
#define SQQUERY_H

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

#include "sqRef.h"
#include "sqStatement.h"

#include <string>
using namespace std;
class sqDatabase;
class sqTable;
class sqRow;
class sqRef;

/// low level: sqQuery is used for running a low level query against the database
class sqQuery {
public:

   /// execute a complete SQL query
   /*! \param db database to query
       \param query the query string */
   sqQuery(sqDatabase* db, const string& query);

   /// execute an insert/update/delete query on the object
   /*! \param db database to query
       \param id persistent id
       \param object object data in form of a complete row
       \param qtype what kind of query to execute */
   sqQuery(sqDatabase* db, const sqRef& id, const sqRow& object, sqQueryType qtype);

   /// execute an insert/update/delete query on the object
   /*! \param db database to query
       \param table table to query against
       \param object object data in form of a complete row
       \param qtype what kind of query to execute */
   sqQuery(sqDatabase* db, sqTable* table, const sqRow& object, sqQueryType qtype);

   /// destructor
   virtual ~sqQuery();

   /// check if query succeeded
   bool ok();

   /// message if something was wrong
   string message();

   // get reference to last inserted row in given table
   /*! \param table table to query
       \return persistent id of last inserted row */
   sqRef row_id(sqTable* table);
private:
   string   m_message;
   bool     m_ok;
};

#endif // SQQUERY_H
