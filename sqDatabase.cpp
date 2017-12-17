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

#include "sqDatabase.h"
#include "sqTable.h"
#include "sqlite3/sqlite3.h"
#include "sqQuery.h"
#include "sqColumn.h"
#include "op_object.h"
#include "sqRow.h"
#include <boost/lexical_cast.hpp>

sqDatabase::sqDatabase(sqlite3* dbh, const string& filename)
: m_dbh(dbh)
, m_filename(filename)
{
   restore_tables();
}

sqDatabase::~sqDatabase()
{
   close();
}

void sqDatabase::close()
{
   clear_tables();
   if(m_dbh) {
      sqlite3_close(m_dbh);
      m_dbh = 0;
   }
   m_filename = "";
}

sqlite3* sqDatabase::dbh()
{
   return m_dbh;
}

string sqDatabase::get_path()
{
   return m_filename;
}

void sqDatabase::clear_tables()
{
   // the tables are owned here, so must be deleted
   auto itable = m_tables.begin();
   while(itable != m_tables.end()) {
      sqTable* table = itable->second;
      itable++;
      delete table;
   }
   // clear the table itself
   m_tables.clear();
}

void sqDatabase::get_tables(list<string>& tables)
{
   // read from master table
   string query = "SELECT name FROM sqlite_master WHERE type='table' ORDER BY name;";

   sqlite3_stmt* statement = 0;
   if(sqlite3_prepare_v2(m_dbh, query.c_str(), -1, &statement, 0) == SQLITE_OK) {

      // traverse the rows in the master table to get the table names
      while(true) {
         if(sqlite3_step(statement) == SQLITE_ROW) {

            // read the table name and push to list
            int icol = 0;
            tables.push_back((const char*)sqlite3_column_text(statement, icol));
         }
         else {
            break;
         }
      }
      sqlite3_finalize(statement);
   }
}

void sqDatabase::restore_tables()
{
   // remoove any definition we may have
   clear_tables();

   list<string> tables;
   get_tables(tables);
   for(auto i=tables.begin(); i!= tables.end(); i++) {
      string tabname = *i;
      m_tables[tabname] = new sqTable(this,tabname);
   }
}

sqTable* sqDatabase::get_table(const string& name)
{
   auto i=m_tables.find(name);
   if(i != m_tables.end()) return i->second;

   return 0;
}

sqTable* sqDatabase::create_table(const string& table_name, const sqRow& columns)
{
    // table exists?
    auto itab = m_tables.find(table_name);
    if(itab != m_tables.end()) {

       sqTable* table = itab->second;

       // if the new def is not compatible, throw exception
       if(!table->is_compatible(columns)) {
          throw logic_error("sqDatabase::create_table, incompatible table defintion detected.");
       }

       // return existing table
       return table;
    }

    // create new table, in database
    sqTable* table = 0;
    sqQuery query(this,columns.table_create_query(table_name));
    if(query.ok()) {

       // note that the table will "clone itself" from the table definition
       // we have just entered in the database
       table = new sqTable(this,table_name);

       // keep the table
       m_tables[table_name] = table;
    }
    else {
      throw logic_error("sqDatabase::create_table, table create failed: " + query.message() );
    }
    return table;
}


bool sqDatabase::make_persistent(op_object* object)
{
   // first make sure the object is not already registered
   if(object->is_persistent()) {
      throw logic_error("op_database::make_persistent, object already persistent");
   }

   // get the persistent layout
   op_values pvalues;
   object->op_layout(pvalues);
   if(pvalues.size() > 0) {

      // ok, we have some values.
      sqRow row(pvalues);

      // Get the table corresponding to the object type
      sqTable* table = get_table(object->op_type());
      if(!table) {
         // the table must be created
         table = create_table(object->op_type(),row);
      }

      // create table entry
      sqRef refid = table->insert_row(row);

      // bind object to the database table entry
      object->bind_persistent(refid);

      string cache_key  = table->name() + '_' + boost::lexical_cast<string>(refid.id());
      op_database* db = table->db();
      db->insert_cached(cache_key,object);

      return true;
   }

   return false;
}

bool sqDatabase::update_persistent(op_object* object)
{
   if(!object->is_persistent()) {
      throw logic_error("op_database::update_persistent, object not persistent: "+ object->op_type());
   }

   // get the persistent layout
   op_values pvalues;
   object->op_layout(pvalues);
   if(pvalues.size() > 0) {

      // Get the table corresponding to the object type
      sqTable* table = get_table(object->op_type());
      if(!table) {
         throw logic_error("op_database::update_persistent, no table for object type: "+ object->op_type());
      }

      if(!table->update_row(object->pid(),pvalues)) {
         throw logic_error("op_database::update_persistent, error updating object: "+ object->op_type() + ", id=" + boost::lexical_cast<string>(object->pid().id()) );
      }

      return true;
   }

   return false;
}

bool sqDatabase::delete_persistent(op_object*& object)
{
   if(!object)return false;

   if(!object->is_persistent()) {
      throw logic_error("op_database::delete_persistent, object not persistent: "+ object->op_type());
   }

   // get the persistent layout, we may have to delete owned objects
   op_values pvalues;
   object->op_layout(pvalues);
   if(pvalues.size() > 0) {
      for(auto i=pvalues.begin(); i!=pvalues.end(); i++) {
         sqColumn* col = *i;
         if(op_object* unique_object = col->get_unique()) {
            delete_persistent(unique_object);
         }
      }
   }

   // Get the table corresponding to the object type
   sqTable* table = get_table(object->op_type());
   if(!table) {
      throw logic_error("op_database::delete_persistent, no table for object type: "+ object->op_type());
   }

   if(!table->erase_row(object->pid())) {
      throw logic_error("op_database::delete_persistent, error erasing object: "+ object->op_type() + ", id=" + boost::lexical_cast<string>(object->pid().id()) );
   }

   // Delete the transient object, its destructor will make sure any cached object is deleted
   delete object;
   object = 0;

   return true;
}

op_object* sqDatabase::restore_persistent(IDint64 pid, op_object* object )
{
   if(!object) {
      throw logic_error("op_database::restore_persistent, cannot restore into null object");
   }

   // first make sure the object is not already persistent
   if(object->is_persistent()) {
      throw logic_error("op_database::restore_persistent, object already persistent: " + object->op_type() + ", id="+ boost::lexical_cast<string>(object->pid().id()));
   }

   // get the persistent layout
   op_values pvalues;
   object->op_layout(pvalues);
   if(pvalues.size() > 0) {

      // Get the table corresponding to the object type
      sqTable* table = get_table(object->op_type());
      if(!table) {
         throw logic_error("op_database::restore_persistent, no table for type: "+ object->op_type());
      }

      // restore the object values from table
      sqRef id(table,pid);
      sqRow row(pvalues);
      if(!table->get_row(id,row)) {
         throw logic_error("op_database::restore_persistent, no such object: " + table->name() + ", id=" + boost::lexical_cast<string>(pid));
      }

      // bind object to the database table entry
      object->bind_persistent(id);
      return object;
   }

   throw logic_error("op_database::restore_persistent, object type has no persistent layout: "+ object->op_type());
   return 0;
}

bool  sqDatabase::select_ids(list<op_pid>& ids, const string& ptype, const string& where_clause)
{
   sqTable* table = get_table(ptype);
   if(!table) return false;

   return table->select_ids(ids,where_clause);
}

bool  sqDatabase::select_min_ids(list<op_pid>& ids, const string& ptype, const string& where_clause)
{
   sqTable* table = get_table(ptype);
   if(!table) return false;

   return table->select_min_ids(ids,where_clause);
}

bool  sqDatabase::select_max_ids(list<op_pid>& ids, const string& ptype, const string& where_clause)
{
   sqTable* table = get_table(ptype);
   if(!table) return false;

   return table->select_max_ids(ids,where_clause);
}
