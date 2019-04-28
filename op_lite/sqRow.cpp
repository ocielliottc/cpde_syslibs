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

#include "sqRow.h"
#include "sqColumn.h"
#include <stdexcept>
#include <map>

sqRow::sqRow()
: m_is_owner(true)
{}

sqRow::sqRow(const sqRow& other)
: m_is_owner(true)
{
   assign_row(other.m_columns,m_is_owner);
}

sqRow::sqRow(const op_values& columns)
: m_is_owner(false)
{
   assign_row(columns,m_is_owner);
}

sqRow& sqRow::operator=(const sqRow& other)
{
   m_is_owner = true;
   assign_row(other.m_columns,m_is_owner);
   return *this;
}

sqRow::~sqRow()
{
   clear();
}

void sqRow::clear()
{
   if(m_is_owner) {
      for(auto i=m_columns.begin(); i!=m_columns.end(); i++) {
         sqColumn* col = i->second;
         delete col;
      }
   }
   m_columns.clear();
}

void sqRow::assign_column(sqColumn* column, bool do_clone)
{
   if(!column) {
      throw logic_error("sqRow::assign_column, null pointer detected.");
   }
   if(m_columns.find(column->colnam()) != m_columns.end()) {
      throw logic_error("sqRow::assign_column, duplicate column name detected: " + column->colnam());
   }

   if(do_clone) {
      sqColumn* col_clone = column->clone();
      m_columns.insert(make_pair(col_clone->colnam(),col_clone));
   }
   else {
      m_columns.insert(make_pair(column->colnam(),column));
   }
}

void sqRow::assign_row(const sqColumnMap& columns, bool do_clone)
{
   clear();
   for(auto i=columns.begin(); i!=columns.end(); i++) {
      sqColumn* column = i->second;
      assign_column(column,do_clone);
   }
}

void sqRow::assign_row(const op_values& columns, bool do_clone)
{
   clear();
   for(auto i=columns.begin(); i!=columns.end(); i++) {
      sqColumn* column = *i;
      assign_column(column,do_clone);
   }
}

size_t sqRow::size() const
{
   return m_columns.size();
}

sqRow::const_iterator sqRow::begin() const
{
   return m_columns.begin();
}

sqRow::const_iterator sqRow::end() const
{
   return m_columns.end();
}

sqRow::const_iterator sqRow::find_column(const string& key) const
{
   return m_columns.find(key);
}

bool sqRow::is_compatible(const sqRow& other) const
{
   if(size() != other.size())return false;

   auto i = begin();
   auto j = other.begin();
   while(i != end()) {
      sqColumn* icol = i->second;
      sqColumn* jcol = j->second;
      if(!icol->is_compatible(jcol))return false;
      i++;
      j++;
   }
   return true;
}

string sqRow::table_create_query(const string& tabname) const
{
   string query = "CREATE TABLE ";
   query += tabname;

   bool first = true;
   map<string,sqColumn*> pk;  // sorted collection of primary keys

   // first collect primary keys and sort them alphabetically
   for(auto icol=m_columns.begin(); icol!=m_columns.end(); icol++) {
      const sqColumn* col = icol->second;
      if(col->pk()) {
         pk.insert(*icol);
      }
   }

   // declare the primary keys columns first
   for(auto ipk=pk.begin(); ipk!=pk.end(); ipk++) {
      const sqColumn* col = ipk->second;
      if(first) {
         query += " (";
      }
      else {
         query += ", ";
      }
      first = false;
      query += col->colnam();
      query += " ";
      query += col->type_string() + " NOT NULL ";
   }

   // then declare non-primary key columns
   for(auto icol=m_columns.begin(); icol!=m_columns.end(); icol++) {
      const sqColumn* col = icol->second;
      if(!col->pk()) {

         if(first) {
            query += " (";
         }
         else {
            query += ", ";
         }

         first = false;
         query += col->colnam();
         query += " ";
         query += col->type_string();
      }
   }


   if(pk.size() > 0) {

      /// finally define primary key(s), these will be mentioned alphabetically
      first = true;
      query += ", PRIMARY KEY (";
      for(auto ipk=pk.begin(); ipk!=pk.end(); ipk++) {
         if(!first)  query += ", ";
         query += (ipk->second)->colnam();
         first = false;
      }
      query += ")";
   }

   // finalise the CREATE TABLE query
   query += ");";

   return query;
}

string sqRow::column_names_csv() const
{
   size_t ncol=0;
   string csv;

   auto icol = m_columns.begin();
   while(icol != m_columns.end()) {

      const sqColumn* col = icol->second;
      if(ncol != 0) csv += ", ";
      csv += col->colnam();
      icol++; ncol++;
   }
   return csv;
}
