#ifndef SQDATABASE_H
#define SQDATABASE_H

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

#include "op_lite_config.h"
#include "op_database.h"

struct sqlite3;
class sqTable;
class sqRow;
#include <list>
#include <unordered_map>
using namespace std;
class op_object;

/// low level: sqDatabase represents low level SQLite database
class OP_LITE_PUBLIC sqDatabase : public op_database {
   friend class sqTable;
   friend class sqQuery;
   friend class sqStatement;
   friend class sqTransaction;
   friend class op_manager;

public:
   typedef unordered_map<string,sqTable*> TableMap;
   typedef TableMap::iterator table_iterator;

   /// destructor
   virtual ~sqDatabase();

   /// close database
   virtual void close();

   /// create persistent object
   /*! \param object transient object with data to instantiate in database */
   bool make_persistent(op_object* object);

   /// update object in database
   /*! \param object transient object with data to update in database */
   bool update_persistent(op_object* object);

   /// erase object in database, delete also transient object
   /*! \param object object to be deleted in database */
   bool delete_persistent(op_object*& object);

   /// restore the persistent object in database, and bind the op_object to it
   /*! \param pid persistent object identifier
       \param object transient object to contain data from database object */
   op_object* restore_persistent(IDint64 pid, op_object* object );

   /// perform SQL query and return a list of persitent identifiers
   /*! \param ids list of persistent identifiers matching query
       \param ptype persistent type string (table name)
       \param where_clause SQL WHERE clause to be evaluated
       \return true when at least one hit */
   bool  select_ids(list<op_pid>& ids, const string& ptype, const string& where_clause = "");

   /// Get minimum id value(s) from type
   /*! \param ids list of persistent identifiers matching query
       \param ptype persistent type string (table name)
       \param where_clause SQL WHERE clause to be evaluated (may be empty)
       \return true when at least one hit */
   bool  select_min_ids(list<sqRef>& ids, const string& ptype, const string& where_clause = "");

   /// Get maximum id value(s) from type
   /*! \param ids list of persistent identifiers matching query
       \param ptype persistent type string (table name)
       \param where_clause SQL WHERE clause to be evaluated (may be empty)
       \return true when at least one hit */
   bool  select_max_ids(list<sqRef>& ids, const string& ptype, const string& where_clause = "");

   /// Get the file system path of the database
   /*! \return path to database file*/
   string get_path();

protected:
   /// \privatesection (hidden in documentation)

   /// create persistent table
   /*! \param table_name name of table to be created
       \param columns column definition for table
       \return table created as sqTable */
   sqTable* create_table(const string& table_name, const sqRow& columns);

   /// return the table definition with the specified name, may return null
   /*! \param table_name name of table to be returned
       \return table as sqTable, or null if not found */
   sqTable* get_table(const string& table_name);

   /// protected constructor only for internal use by database manager
   /*! \param dbh SQLite database handle
       \param filename database file name (may include path) */
   sqDatabase(sqlite3* dbh, const string& filename);

   /// clear member table map
   void clear_tables();

   /// restore table names from database
   /*! \param tables list of table names as found in database */
   void get_tables(list<string>& tables);

   /// restore all tables from database into table map
   void restore_tables();

   /// return low level database handle
   sqlite3* dbh();

private:
   sqlite3* m_dbh;      // Sqlite database handle
   string   m_filename; // filename of database (valid only when m_dbh is valid)
   TableMap m_tables;   // tables of this database
};

#endif // SQDATABASE_H
