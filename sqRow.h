#ifndef SQROW_H
#define SQROW_H

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

#include <map>
#include "op_lite_config.h"
#include "sqColumn.h"

/// low level: sqRow represents a collection of sqColumn
class OP_LITE_PUBLIC sqRow {
   friend class sqTable;
   friend class sqDatabase;

public:
   typedef map<string,sqColumn*> sqColumnMap;
   typedef sqColumnMap::const_iterator const_iterator;

   /// default constructor
   sqRow();

   /// copy constructor
   sqRow(const sqRow& other);

   /// construct row from op_values columns
   sqRow(const op_values& columns);

   /// assignment operator
   sqRow& operator=(const sqRow& other);

   /// destructor
   virtual ~sqRow();

   /// clear all contents
   void clear();

   /// return number of columns
   size_t size() const;

   /// return iterator for first column
   const_iterator begin() const;
   /// return iterator for one beyond last column
   const_iterator end() const;

   /// get an iterator pointing to the named column
   const_iterator find_column(const string& key) const;

   /// return true if other row has the exact same column name & types as this row
   bool is_compatible(const sqRow& other) const;

protected:
   /// \privatesection (hidden in documentation)

   /// assign a single column to this row (pointer will be cloned or not, depending on clone flag)
   void assign_column(sqColumn* column, bool do_clone);

   /// assign columns contained in the map
   void assign_row(const sqColumnMap& columns, bool do_clone);

   /// assign columns contained in op_values;
   void assign_row(const op_values& columns, bool do_clone);

   /// create query string for table creation
   string table_create_query(const string& tabname) const;

   /// return list of column names as a comma separated string (used for creating queries)
   string column_names_csv() const;

private:
   sqColumnMap m_columns;
   bool        m_is_owner;  // true when this object owns the values in m_columns
};

#endif // SQROW_H
