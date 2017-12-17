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


#include "op_manager.h"
#include "op_version.h"
#include <fstream>
using namespace std;

#include "sqlite3/sqlite3.h"
#include "sqDatabase.h"
#include "sqQuery.h"

op_manager op_manager::m_self;

string op_manager::version()
{
   return OP_LITE_version;
}

op_manager::op_manager()
{
   m_dbmap_sel = m_dbmap.end();
}

op_manager* op_manager::singleton()
{
   return &m_self;
}

op_manager::~op_manager()
{
   clear();
}

bool op_manager::file_exists(const string& path)
{
   ifstream in(path,ios_base::in|ios_base::binary);
   return in.good();
}

op_database* op_manager::create_database(const string& logical_name, const string& file_path)
{
   // first check logical name
   auto i = m_dbmap.find(logical_name);
   if(i != m_dbmap.end())return 0;
/*
   Pending update with exception

   if(i != m_dbmap.end()) {
      string msg = typeid(this).name();
      msg += ", cannot create database as logical name already in use: " + logical_name;
      throw logic_error(msg);
   }
*/

   // then check file existence
   if(file_exists(file_path)) return 0;
/*
   Pending update with exception

   if(file_exists(file_path)) {
      string msg = typeid(this).name();
      msg += ", cannot create database as file already exists: " + file_path;
      throw logic_error(msg);
   }
*/

   // OK, we are good to go
   sqlite3* db_handle = 0;
   sqDatabase* db = 0;
   if(sqlite3_open(file_path.c_str(), &db_handle) == SQLITE_OK) {

      // ok, register the database
      db = new sqDatabase(db_handle,file_path);
      m_dbmap[logical_name] = db;

      // set page size
      sqQuery(db,string("PRAGMA page_size=16384"));

      // set the selected database
      m_dbmap_sel = m_dbmap.find(logical_name);
   }
   return db;
}


op_database* op_manager::open_database(const string& logical_name, const string& file_path)
{
   // first check logical name
   auto i = m_dbmap.find(logical_name);
   if(i != m_dbmap.end())return 0;

   // then check file existence
   if(!file_exists(file_path)) return 0;

   // OK, we are good to go
   sqlite3* db_handle = 0;
   op_database* db = 0;
   if(sqlite3_open(file_path.c_str(), &db_handle) == SQLITE_OK) {

      // ok, register the database
      db = new sqDatabase(db_handle,file_path);
      m_dbmap[logical_name] = db;

      // set the selected database
      m_dbmap_sel = m_dbmap.find(logical_name);
   }
   return db;
}

op_database* op_manager::open_create_database(const string& logical_name, const string& file_path)
{
   // first check logical name
   auto i = m_dbmap.find(logical_name);
   if(i != m_dbmap.end())return 0;

   // then check file existence
   bool existing_db = file_exists(file_path);

   // OK, we are good to go
   sqlite3* db_handle = 0;
   sqDatabase* db = 0;
   if(sqlite3_open(file_path.c_str(), &db_handle) == SQLITE_OK) {

      // ok, register the database
      db = new sqDatabase(db_handle,file_path);
      m_dbmap[logical_name] = db;

      if(!existing_db) sqQuery(db,"PRAGMA page_size=16384");

      // set the selected database
      m_dbmap_sel = m_dbmap.find(logical_name);
   }

   return db;
}


op_database*  op_manager::close_database(const string&  logical_name)
{
   if(op_database* db = get_database(logical_name)) {
      db->flush_cached(true,true);

      if(selected_database() == db) {
         // we are closing the selected database, so clear that setting
         m_dbmap_sel = m_dbmap.end();
      }

      delete db;
      m_dbmap.erase(logical_name);
   }
   return selected_database();
}

op_database* op_manager::get_database(const string& logical_name)
{
   // first check logical name
   auto i = m_dbmap.find(logical_name);
   if(i == m_dbmap.end())return 0;

   return i->second;
}


op_database* op_manager::select_database(const string& logical_name)
{
   auto i = m_dbmap.find(logical_name);
   if(i != m_dbmap.end()) {
      m_dbmap_sel = i;
      return i->second;
   }
   m_dbmap_sel = m_dbmap.end();
   return 0;
}

 op_database* op_manager::select_database(op_database* db)
 {
   for(auto i=m_dbmap.begin(); i!=m_dbmap.end(); i++) {
      if(db == i->second) {
         m_dbmap_sel = i;
         return i->second;
      }
   }
   m_dbmap_sel = m_dbmap.end();
   return 0;
 }

op_database* op_manager::selected_database()
{
   if(m_dbmap_sel != m_dbmap.end())return m_dbmap_sel->second;
   return 0;
}

string op_manager::selected_database_logical_name()
{
   if(m_dbmap_sel != m_dbmap.end())return m_dbmap_sel->first;
   return "";
}

op_type_factory* op_manager::type_factory()
{
   return &m_type_factory;
}

void op_manager::clear()
{
   m_dbmap_sel = m_dbmap.end();
   for(auto i=m_dbmap.begin(); i!=m_dbmap.end(); i++) {
      op_database* db = i->second;
      delete db;
   }
   m_dbmap.clear();
}
