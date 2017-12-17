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

#include <memory>
#include <sstream>
using namespace std;

#include "sqlite3/sqlite3.h"

#include "sqDatabase.h"
#include "sqTable.h"
#include "sqValue.h"
#include "sqRef.h"
#include "sqStatement.h"
#include "sqQuery.h"
#include "op_packed.h"

std::unique_ptr<sqColumn> column_from_string_type(const string& colnam, const string& stype, sqPK pk)
{
   // http://www.sqlite.org/datatype3.html
   // http://www.sqlite.org/c3ref/c_blob.html
   int    ival=0;
   string sval;
   double dval=0.0;

   if(stype.find("INT") != string::npos)       return std::unique_ptr<sqColumn>(new sqValue<int>(colnam,ival,pk));
   else if(stype.find("CHAR") != string::npos) return std::unique_ptr<sqColumn>(new sqValue<string>(colnam,sval,pk));
   else if(stype.find("CLOB") != string::npos) return std::unique_ptr<sqColumn>(new sqValue<string>(colnam,sval,pk));
   else if(stype.find("TEXT") != string::npos) return std::unique_ptr<sqColumn>(new sqValue<string>(colnam,sval,pk));
   else if(stype.find("BLOB") != string::npos) return std::unique_ptr<sqColumn>(new sqValue<sqBlob>(colnam,sqBlob(),pk));
   else if(stype.find("REAL") != string::npos) return std::unique_ptr<sqColumn>(new sqValue<double>(colnam,dval,pk));
   else if(stype.find("FLOA") != string::npos) return std::unique_ptr<sqColumn>(new sqValue<double>(colnam,dval,pk));
   else if(stype.find("DOUB") != string::npos) return std::unique_ptr<sqColumn>(new sqValue<double>(colnam,dval,pk));
   else                                        return std::unique_ptr<sqColumn>();
}


sqTable::sqTable(sqDatabase* db,const string& tname)
: m_db(db)
, m_tname(tname)
{
  restore_metadata();
}

sqTable::~sqTable()
{}

string sqTable::name() const
{
   return m_tname;
}

sqDatabase* sqTable::db()
{
   return m_db;
}


sqRef sqTable::insert_row(const sqRow& values)
{
   if(!m_row.is_compatible(values)) {
      throw logic_error("sqTable::insert_row, incompatible table defintion detected. " + m_tname);
   }

   sqQuery query(m_db,this,values,sqRECORD_INSERT);
   if(!query.ok()) {
      throw logic_error("sqTable::insert_row, insert query failed: " + m_tname + ' ' + query.message() );
   }
   return query.row_id(this);
}

bool sqTable::update_row(const sqRef& id, const sqRow& values)
{
   if(!m_row.is_compatible(values)) {
      throw logic_error("sqTable::update_row, incompatible table defintion detected. "  + m_tname);
   }

   sqQuery query(m_db,id,values,sqRECORD_UPDATE);
   if(!query.ok()) {
      throw logic_error("sqTable::update_row, update query failed: " + m_tname + ' '+ query.message());
   }

   return true;
}

bool sqTable::erase_row(const sqRef& id)
{
   sqQuery query(m_db,delete_query_string(id));
   if(!query.ok()) {
       throw logic_error("sqTable::erase_row, delete query failed.");
   }
   return true;
}

bool sqTable::is_compatible(const sqRow& row) const
{
   return m_row.is_compatible(row);
}

void sqTable::restore_metadata()
{
   m_row.clear();

   // read one record from the sqlite3 table
   string query = "SELECT * FROM " + m_tname + " LIMIT 1;";
   sqlite3_stmt* statement = 0;

   // parse the statement
   if(sqlite3_prepare_v2(m_db->dbh(), query.c_str(), -1, &statement, 0) == SQLITE_OK) {

      // get number of columns in table
      int ncols = sqlite3_column_count(statement);
      int npk = 0;
      for(int icol = 0; icol < ncols; icol++) {

         // get column name
         string colnam = sqlite3_column_name(statement, icol);

         // empty tables will result in no type info for the columns, but
         // we have some metadata available

         char data_type[512];
         const char* stype = &data_type[0];
         int pk = 0;   // true if primary key

         sqlite3_table_column_metadata(m_db->dbh(),0,m_tname.c_str(),colnam.c_str(),&stype,0,0,&pk,0);

         /// WARNING: composite primary keys are not handled reliably here
         ///          There is no way to detect the order of the primary keys
         ///          Workaround: name them alphabetically to indicate the order (or better: don't use composite keys)
         sqPK pk_e = sqPK_NONE;
         if(pk>0) {
            pk_e = (npk==0)? sqPK_1 : sqPK_2;
            npk++;
         }

         // create a column type as specified in the CREATE TABLE statement
         std::unique_ptr<sqColumn> col = column_from_string_type(colnam,stype,pk_e);

         // save the column info
         m_row.assign_column(col.get(),true);
      }

      sqlite3_finalize(statement);
   }
}

string sqTable::insert_query_string() const
{
   // INSERT INTO table_name (column1, column2, column3,...)
   // VALUES (value1, value2, value3,...)

   ostringstream query;
   ostringstream values;

   query  << "INSERT INTO "<< m_tname << " (";
   values << " VALUES (";

   bool first = true;
   for(auto icol=m_row.begin(); icol!=m_row.end(); icol++) {
      const string& cname = icol->first;
      const sqColumn* col = icol->second;

      // preceding comma separator unless first parameter
      if(!first) {
         query << ',';
         values << ',';
      }

      first = false;
      query  << cname;
      values << '?';
   }

   // close the statements
   query << ") ";
   values << ") ";

   // append the VALUES part to the whole query
   query << values.str() << ';';
   return query.str();
}

string sqTable::update_query_string(const sqRef& ref) const
{
   // UPDATE table_name
   // SET column1=value, column2=value2,...
   // WHERE some_column=some_value

   ostringstream query;
   query << "UPDATE " << m_tname << " SET ";

   bool first = true;
   for(auto icol=m_row.begin(); icol!=m_row.end(); icol++) {

      const string& cname = icol->first;
      const sqColumn* col = icol->second;
      if(sqPK_NONE==col->pk()) {

          if(!first) {
            query << ", ";
          }
          query << cname << "=?";
          first = false;
      }
   }
   query << " WHERE ROWID=" << ref.id() <<';';
   return query.str();
}

string sqTable::delete_query_string(const sqRef& ref) const
{
   // DELETE FROM table_name
   // WHERE some_column=some_value

   ostringstream query;
   query << "DELETE FROM " << m_tname << " WHERE ROWID=" << ref.id();
   return query.str();
}


bool sqTable::get_row(const sqRef& id, sqRow& values)
{
   if(id.id() < 1)return false;

   ostringstream query;
   query << "SELECT " << values.column_names_csv() << " FROM " << this->name() << " WHERE ROWID=" << id.id() << ";";
   string qstr = query.str();

   int nmatch = 0;
   sqlite3_stmt* statement = 0;
   if(sqlite3_prepare_v2(m_db->dbh(), qstr.c_str(), -1, &statement, 0) == SQLITE_OK) {
      int ncols = sqlite3_column_count(statement);
      while(true) {
         if(sqlite3_step(statement) == SQLITE_ROW) {
            for(int icol = 0; icol < ncols; icol++) {
               string colnam = sqlite3_column_name(statement, icol);

               // does this column name match?
               auto itcol = values.find_column(colnam);
               if(itcol != values.end()) {

                  sqColumn* col = itcol->second;

                  // ok, name matched. What about SQLite column type?
                  if(col->sqlite3_type() == sqlite3_column_type(statement,icol)) {

                     // type matched also, NOTE: the actual type() may be different from sqlite3_type() (specialisation)
                     switch(col->type()) {
                        case sqINTEGER:
                              {
                                 nmatch++;
                                 sqValue<IDint64>* val = dynamic_cast<sqValue<IDint64>*>(col);
                                 val->set_value(sqlite3_column_int64(statement, icol));
                                 break;
                              }
                        case sqDOUBLE:
                              {
                                 nmatch++;
                                 sqValue<double>* val = dynamic_cast<sqValue<double>*>(col);
                                 val->set_value(sqlite3_column_double(statement, icol));
                                 break;
                              }
                        case sqTEXT:
                              {
                                 nmatch++;
                                 sqValue<string>* val = dynamic_cast<sqValue<string>*>(col);
                                 string text  = (const char*)sqlite3_column_text(statement, icol);
                                 val->set_value(text);
                                 break;
                              }
                        case sqBLOB:
                              {
                                 nmatch++;
                                 sqValue<sqBlob>* val = dynamic_cast<sqValue<sqBlob>*>(col);
                                 int nbytes      = sqlite3_column_bytes(statement,icol);
                                 const unsigned char* blob_data = (const unsigned char*)sqlite3_column_blob(statement,icol);
                                 val->set_value(sqBlob(blob_data,nbytes));
                                 break;
                              }
                        case sqBLOB_PACKED:
                              {
                                 nmatch++;
                                 if(op_packed* val  = dynamic_cast<op_packed*>(col) ) {
                                    int nbytes   = sqlite3_column_bytes(statement,icol);
                                    const unsigned char* blob_data = (const unsigned char*)sqlite3_column_blob(statement,icol);
                                    val->unpack(sqBlob(blob_data,nbytes));
                                 }
                                 break;
                              }
                        default:
                              {}
                     }
                  }
               }
            }
         }
         else {
            break;
         }
      }
      sqlite3_finalize(statement);
   }

   return (nmatch>0);
}

bool  sqTable::select_query(list<sqRef>& ids, const string& query)
{
   size_t count=0;
   sqlite3_stmt* statement = 0;
   if(sqlite3_prepare_v2(m_db->dbh(), query.c_str(), -1, &statement, 0) == SQLITE_OK) {

      int icol = 0;
      while(true) {
         if(sqlite3_step(statement) == SQLITE_ROW) {
            ids.push_back(sqRef(this,sqlite3_column_int64(statement, icol)));
            count++;
         }
         else {
            break;
         }
      }
      sqlite3_finalize(statement);
   }
   return (count>0);
}

bool  sqTable::select_ids(list<sqRef>& ids, const string& where_clause)
{
   string query = "SELECT ROWID FROM " + name() + ' ' + where_clause + ';';

   return select_query(ids,query);
}

bool  sqTable::select_min_ids(list<sqRef>& ids, const string& where_clause)
{
   string query = "SELECT MIN(ROWID) FROM " + name() + ' ' + where_clause + ';';

   return select_query(ids,query);
}

bool  sqTable::select_max_ids(list<sqRef>& ids, const string& where_clause)
{
   string query = "SELECT MAX(ROWID) FROM " + name() + ' ' + where_clause + ';';

   return select_query(ids,query);
}


