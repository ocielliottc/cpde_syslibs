#ifndef SQTABLE_H
#define SQTABLE_H

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

#include <string>
#include <list>
using namespace std;

#include "op_lite_config.h"
#include "sqRow.h"
#include "sqRef.h"
class sqDatabase;

/// low level: sqTable represents a table definition
class OP_LITE_PUBLIC sqTable {
   friend class sqDatabase;
   friend class sqQuery;
   friend class sqStatement;

public:

   /// default constructor
   sqTable();

   /// destructor
   virtual ~sqTable();

   /// return table name
   string name() const;

   /// create new record in table, return id for the new record
   /*!  \param  values  the row of column values
        \return persistent id (rowid) of record created */
   sqRef insert_row(const sqRow& values);

   /// update existing record in table
   /*!  \param  id persistent id (rowid) of record to update
        \param  values all or selected columns to update values for
        \return true when success */
   bool  update_row(const sqRef& id, const sqRow& values);

   /// delete existing record in table
   /*!  \param  id persistent id (rowid) to erase
        \return true when success */
   bool  erase_row(const sqRef& id);

   /// get values of existing record.
   /*!  \param  id persistent id (rowid) to erase
        \param  values  the row of column values */
   bool  get_row(const sqRef& id, sqRow& values);

   /// return database the table belongs to
   sqDatabase* db();

   /// perform SQL query and return a list of persistent identifiers
   /*! \param ids list of persistent identifiers matching query
       \param where_clause SQL WHERE clause to be evaluated (empty where_clause returns all)
       \return true when at least one hit */
   bool  select_ids(std::list<sqRef>& ids, const string& where_clause = "");

   /// Get minimum id value(s) from table
   /*! \param ids list of persistent identifiers matching query
       \param where_clause SQL WHERE clause to be evaluated (may be empty)
       \return true when at least one hit */
   bool  select_min_ids(std::list<sqRef>& ids, const string& where_clause = "");

   /// Get maximum id value(s) from table
   /*! \param ids list of persistent identifiers matching query
       \param where_clause SQL WHERE clause to be evaluated (may be empty)
       \return true when at least one hit */
   bool  select_max_ids(std::list<sqRef>& ids, const string& where_clause = "");

protected:
   /// \privatesection (hidden in documentation)

   /// run a select query
   bool  select_query(std::list<sqRef>& ids, const string& query);

   /// restore the table from given database
   sqTable(sqDatabase* db, const string& tname);

   /// restore table metadata into m_row
   void restore_metadata();

   /// check if given row has the exact same column names & types as this row
   bool is_compatible(const sqRow& row) const;

   /// construct query string for insert,update and delete
   string insert_query_string() const;
   string update_query_string(const sqRef& ref) const;
   string delete_query_string(const sqRef& ref) const;

private:
   sqDatabase* m_db;      // the database where the table lives
   string      m_tname;   // the name of the table
   sqRow       m_row;     // table metadata
};

#endif // SQTABLE_H
