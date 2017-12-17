#ifndef SQREF_H
#define SQREF_H

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
#include "op_lite_config.h"

class sqTable;
#include <string>
using namespace std;

/// low level: sqRef represents a transient reference to a table entry, has table and rowid for entry
class OP_LITE_PUBLIC sqRef {
public:

   /// default constructor
   sqRef();

   /// constructor
   /*! \param table database table */
   sqRef(sqTable* table);

   /// constructor
   /*! \param table database table
       \param id persistent object identifier (rowid) */
   sqRef(sqTable* table, IDint64 id);

   /// copy constructor
   /*! \param other other reference to copy from */
   sqRef(const sqRef& other);

   /// destructor
   virtual ~sqRef();

   /// return the table referenced
   sqTable* table()  const { return m_table; }

   /// return the record id referenced
   IDint64 id() const { return m_id; }

   /// true if reference does not refer to any object
   bool is_null() const     { return m_id== -1; }

private:
   sqTable* m_table;
   IDint64 m_id;
};

#endif // SQREF_H
