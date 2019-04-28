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

#include "sqStatement.h"
#include "sqDatabase.h"
#include "sqTable.h"
#include "sqRow.h"
#include "sqValue.h"
#include "op_packed.h"

sqStatement::sqStatement(sqDatabase* db, const sqRef& ref, const sqRow& object, sqQueryType qtype)
: m_db(db)
, m_ref(ref)
, m_qtype(qtype)
, m_stmt(0)
, m_query("")
, m_message("")
, m_ok(false)
{
   m_ok = execute_statement(object);
}

sqStatement::~sqStatement()
{
   if(m_stmt) {
      sqlite3_finalize(m_stmt);
      m_stmt = 0;
   }
}

bool sqStatement::execute_statement(const sqRow& object)
{
   bool bind = false;
   bool insert = false;
   bool update = false;
   bool bind_ok = false;

   sqTable* table = m_ref.table();

   switch(m_qtype) {
      case sqRECORD_INSERT: { m_query = table->insert_query_string();      bind=true;  insert=true;  update=false; break; }
      case sqRECORD_UPDATE: { m_query = table->update_query_string(m_ref); bind=true;  insert=false; update=true;  break; }
      case sqRECORD_DELETE: { m_query = table->delete_query_string(m_ref); bind=false; insert=false; update=false; break; }
      default:            { return false; }
   };

   // parse the statement
   if(sqlite3_prepare_v2(m_db->dbh(), m_query.c_str(), -1, &m_stmt, 0) == SQLITE_OK) {

      if(bind) {
         //bind values to sql parameters
         int index = 1;
         for(auto icol = object.begin(); icol != object.end(); icol++) {

            sqColumn* col = icol->second;
            sqValueType vtype = col->type();

            switch(vtype) {
               case sqINTEGER:
                     {
                        sqValue<IDint64>* val = dynamic_cast<sqValue<IDint64>*>(col);
                        if(insert && (val->pk() != sqPK_NONE) && val->value()<1) {
                           // This is a primary key integer with non-positive value during INSERT,
                           // meaning it shall be generated.
                           // Autogenerate key value by binding to null
                           bind_ok = (SQLITE_OK == sqlite3_bind_null(m_stmt,index++));
                        }
                        else if(update && (val->pk() != sqPK_NONE)) {
                           // primary key during UPDATE is ignored
                        }
                        else {
                           // normal integer, or not insert
                           bind_ok = (SQLITE_OK == sqlite3_bind_int64(m_stmt,index++,val->value()));
                        }
                        break;
                     }
               case sqDOUBLE:
                     {
                        sqValue<double>* val = dynamic_cast<sqValue<double>*>(col);
                        if(update && (val->pk() != sqPK_NONE)) {
                           // primary key during UPDATE is ignored
                        }
                        else {
                           bind_ok = (SQLITE_OK == sqlite3_bind_double(m_stmt,index++,val->value()));
                        }
                        break;
                     }
               case sqTEXT:
                     {
                        sqValue<string>* val = dynamic_cast<sqValue<string>*>(col);
                        if(update && (val->pk() != sqPK_NONE)) {
                           // primary key during UPDATE is ignored
                        }
                        else {
                           int nbytes = val->value().length()*sizeof(char);
                           bind_ok = (SQLITE_OK == sqlite3_bind_text(m_stmt,index++,val->value().c_str(),nbytes,0));
                        }
                        break;
                     }
               case sqBLOB:
                     {
                        sqValue<sqBlob>* val = dynamic_cast<sqValue<sqBlob>*>(col);
                        const sqBlob& blob = val->value();
                        size_t bsiz = blob.packed_size();
                        bind_ok = (SQLITE_OK == sqlite3_bind_blob(m_stmt,index++,blob.data(),bsiz,0));
                        break;
                     }
               case sqBLOB_PACKED:
                     {
                        if(op_packed* val = dynamic_cast<op_packed*>(col)) {
                           // pack to a transient buffer, make sure SQLite knows it is transient
                           sqBlob blob;
                           val->pack(blob);
                           if(blob.packed_size() > 0) {
                              bind_ok = (SQLITE_OK == sqlite3_bind_blob(m_stmt,index++,blob.data(),blob.packed_size(),SQLITE_TRANSIENT));
                           }
                           else {
                              bind_ok = (SQLITE_OK == sqlite3_bind_blob(m_stmt,index++,0,0,SQLITE_TRANSIENT));
                           }
                        }
                        break;
                     }
               default:
                     {
                        bind_ok = false;
                     }
            }
         }
      }
      else {
         bind_ok = true;  // no binding attempted
      }

      if(bind_ok) {
         if(SQLITE_DONE == sqlite3_step(m_stmt)) {
            m_ok = true;
         }
      }
   }
   else {
      m_message = sqlite3_errmsg(m_db->dbh());
   }
   return m_ok;
}



bool sqStatement::ok()
{
   return m_ok;
}

string sqStatement::message()
{
   return m_message;
}
