#ifndef OP_DATABASE_H
#define OP_DATABASE_H

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

#include <boost/bimap.hpp>
#include <memory>
#include <list>
using namespace std;

#include "op_lite_config.h"
#include "op_value.h"

// op_database uses boost bimap ( = "bidirectional map")
//  http://www.boost.org/doc/libs/1_42_0/libs/bimap/doc/html/index.html

struct sqlite3;
class op_transaction;
class op_object;

/// An op_database represents a single op_lite database
class OP_LITE_PUBLIC op_database {
   friend class op_manager;
   friend class op_transaction;

public:
   virtual ~op_database();

   /// make the object persistent in this database, and bind the op_object to it
   /*!  \param  object object to become persistent
        \return true when success */
   virtual bool make_persistent(op_object* object) = 0;

   /// update object in database
   /*!  \param  object object containing updated values
        \return true when success */
   virtual bool update_persistent(op_object* object) = 0;

   /// erase object in database, delete also transient object
   /*!  \param  object object to be deleted
        \return true when success */
   virtual bool delete_persistent(op_object*& object) = 0;

   /// restore the object persistent in this database, and bind the op_object to it
   /*! \param  pid persistent identifier of object to be restored
       \param  object object to be restored from database
       \return restored object */
   virtual op_object* restore_persistent(IDint64 pid, op_object* object )  = 0;

   /// get a cached object via string key (NOTE: may return 0);
   /*! \param  key cache lookup key
       \return object returned from cache, or 0 if not in cache */
   op_object* get_cached(const string& key);

   /// insert a persistent object into the cache
   /*! \param  key cache lookup key
       \param  object to be inserted into cache */
   bool insert_cached(const string& key, op_object* object);

   /// erase persistent object from cache
   /*! \param  object to be erased from cache
       \return true if success */
   void erase_cached(op_object* object);

   /// flush all cached objects to database
   /*! \param update_database true if cache values to be reflected in database
       \param delete_transient true if transient object to be deleted after flush */
   void flush_cached(bool update_database, bool delete_transient);

   /// return low level database handle
   virtual  sqlite3* dbh() = 0;

   /// get the file system path of the database
   /*! \return path to database file*/
   virtual string get_path() = 0;


   /// check existence of persistent object identified by type and id
   /*! \param  ptype tupe of object
       \param  pid persistent identifier of object
       \return true if object exists */
   bool object_exists(const string& ptype, IDint64 pid );

   /// perform SQL query and return a list of persitent identifiers
   /*! \param ids list of persistent identifiers matching query
       \param ptype persistent type string (table name)
       \param where_clause SQL WHERE clause to be evaluated (empty clause returns all)
       \return true when at least one hit */
   virtual bool  select_ids(list<op_pid>& ids, const string& ptype, const string& where_clause = "") = 0;

   /// Get minimum id value(s) from type
   /*! \param ids list of persistent identifiers matching query
       \param ptype persistent type string (table name)
       \param where_clause SQL WHERE clause to be evaluated (may be empty)
       \return true when at least one hit */
   virtual bool  select_min_ids(list<op_pid>& ids, const string& ptype, const string& where_clause = "") = 0;

   /// Get maximum id value(s) from type
   /*! \param ids list of persistent identifiers matching query
       \param ptype persistent type string (table name)
       \param where_clause SQL WHERE clause to be evaluated (may be empty)
       \return true when at least one hit */
   virtual bool  select_max_ids(list<op_pid>& ids, const string& ptype, const string& where_clause = "") = 0;

   /// restore table names from database
   /*! \param tables list of table names as found in database */
   virtual void get_tables(list<string>& tables) = 0;

protected:
   /// \privatesection (hidden in documentation)

   /// default constructor
   op_database();

   /// close the database file, be sure to update objects first, or data will be lost!
   virtual void close() = 0;

   /// push and pop ensure support for nested transactions
   void push_transaction(op_transaction* trans);
   void pop_transaction();

private:
   typedef list<op_transaction*> TransactionList;
   TransactionList  m_tlist;

   // using boost bi-directional map, where both values are lookup keys
   typedef boost::bimap<string,op_object*>  ObjectMap;
   ObjectMap  m_objects;
};

#endif // OP_DATABASE_H
