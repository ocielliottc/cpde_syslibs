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

#include "op_database_helper.h"
#include "op_manager.h"
#include <stdexcept>

op_database_helper::op_database_helper(const string& logical_name, const string& file_path, bool readonly)
: m_logical_name(logical_name)
, m_file_path(file_path)
, m_readonly(readonly)
, m_db(0)
{
   if(m_readonly) {
      m_db = op_mgr()->open_database(m_logical_name,m_file_path);
   }
   else {
      m_db = op_mgr()->open_create_database(m_logical_name,m_file_path);
   }
   if(!m_db)throw std::logic_error("op_lite database could not be opened: " + m_file_path);
}

op_database_helper::~op_database_helper()
{
   close();
}

void op_database_helper::close()
{
   bool update_database = true;
   bool delete_transient = true;
   if(m_db && m_readonly) {
      // do NOT write them to the database
      update_database = false;
   }
   m_db = op_mgr()->close_database(m_logical_name,update_database);
}
