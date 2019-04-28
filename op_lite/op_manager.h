#ifndef OP_MANAGER_H
#define OP_MANAGER_H

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

#include <string>
#include <map>
using namespace std;
#include "op_lite_config.h"
#include "op_type_factory.h"
class op_database;

/// An op_manager manages the op_lite databases, plus their common type factory
class OP_LITE_PUBLIC op_manager {
public:
   /// Return the one and only op_manager siingleton object. See also the op_mgr() inline helper function.
   static op_manager* singleton();

   /// op_lite library version id
   /*! \return version id on the form "VM.mm-cc" (M=major,mm=minor,cc=cycle)  */
   string version();

   /// flush and close all open databases
   void clear();

   /// destructor, calls clear()
   virtual ~op_manager();

   /// Check if database file exist before opening new/old database
   bool file_exists(const string& file_path);

   /// create a NEW database file, fail if logical name already exists or the file already exists
   /*! \param logical_name logical name of database
       \param file_path path to database file
       \return database or null if error, the database becomes the selected database on success*/
   op_database* create_database(const string& logical_name, const string& file_path);

   /// open an EXISTING database file, fail if logical name already exists or the file does not exists
   /*! \param logical_name logical name of database
       \param file_path path to database file
       \return database or null if error, the database becomes the selected database on success*/
   op_database* open_database(const string& logical_name, const string& file_path);

   /// Open existing or create new database file, fail if logical name already exists
   /*! \param logical_name logical name of database
       \param file_path path to database file
       \return database or null if error, the database becomes the selected database on success*/
   op_database* open_create_database(const string& logical_name, const string& file_path);

   /// Close open database, all cached objects will be flushed to db (if update_database=true)
   /*! \param logical_name logical name of database
       \return database after close, normally 0 */
   op_database* close_database(const string& logical_name, bool update_database);

   /// Close all open databases, all cached objects will be flushed to db (if update_database=true)
   void close_all_databases(bool update_database);

   /// Cet pointer to open database with given logical name, fail if logical name does no exist
   /*! \param logical_name logical name of database
       \return database or null if error  */
   op_database* get_database(const string& logical_name);

   /// Select a database as the current one, using an existing logical name.
   /*! \param logical_name logical name of database
       \return selected database or null if error  */
   op_database* select_database(const string& logical_name);

   /// Select a database as the current one, using a db pointer
   /*! \param db one of the existing databases
       \return selected database or null if error  */
   op_database* select_database(op_database* db);

   /// return currently selected database, may return 0
   op_database* selected_database();

   /// return currently selected database logical name, may return blank
   string       selected_database_logical_name();

   /// provide access to the type factory
   op_type_factory* type_factory();

protected:
   /// \privatesection (hidden in documentation)

   op_manager();
   static op_manager m_self;

protected:
   typedef map<string,op_database*> DatabaseMap;

private:
   DatabaseMap             m_dbmap;
   DatabaseMap::iterator   m_dbmap_sel;     // selected database in m_dbmap
   op_type_factory         m_type_factory;  // object type factory, allowing for persistent polymorphic types
};

/// convenience access to op_manager singleton object
inline op_manager* op_mgr() { return op_manager::singleton(); }

#endif // OP_MANAGER_H
