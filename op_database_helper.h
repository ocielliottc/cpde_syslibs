#ifndef OP_DATABASE_HELPER_H
#define OP_DATABASE_HELPER_H

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

#include "op_lite_config.h"
#include "op_database.h"

/// An helper object encapsulating an op_database & making it exception safe. Closes db when it goes out of scope
class OP_LITE_PUBLIC op_database_helper {
public:
   /// Open an existing database or optionally create new (if readonly=false)
   /*! \param logical_name logical name of database
       \param file_path path to database file
       \param readonly true will disable any write backs
       \return database or null if error, the database becomes the selected database on success*/
   op_database_helper(const string& logical_name, const string& file_path, bool readonly=false);
   virtual ~op_database_helper();

   /// pointer like syntax to get database object
   op_database* operator->() { return m_db; }

   /// read only access
   const op_database* operator->() const { return m_db; }

   // get syntax to access encapsulated database
   op_database* get() { return m_db; }

   // get syntax to access encapsulated database as const
   const op_database* get() const { return m_db; }

   // close database
   void close();

protected:
private:
   string       m_logical_name;
   string       m_file_path;
   bool         m_readonly;
   op_database* m_db;
};

#endif // OP_DATABASE_HELPER_H
