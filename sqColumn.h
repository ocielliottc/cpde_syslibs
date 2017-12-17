#ifndef SQCOLUMN_H
#define SQCOLUMN_H

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

#include <set>
#include <vector>
#include <memory>
#include <string>
using namespace std;
#include "op_lite_config.h"
class op_object;


// sqColumn: Represents a a column in a table definition
//    - has enough info to define the column when table is created
//    - classes derived from sqColumn contain values

// Primary key info
// WARNING: when using composite keys (PK_1,PK_2) together, the column names MUST be such that
//          the PK_1 name sorts alphabetically before the PK_2 name. This is due to a shortcoming
//          in SQLite's ability to retrieve table metadata

/// sqPK enum, Primary key status
enum sqPK  { /// sqPK_NONE  not a primary key
             sqPK_NONE = 0,
             ///  sqPK_1  1st primary key  (also used when only one key)
             sqPK_1    = 1,
             ///  sqPK_2 2nd primary key  (only for composite keys)
             sqPK_2    = 2
            };


/// sqValueType enum, Basic persistent types supported. Note the sqBLOB extensions
enum sqValueType { // === first basic types maching SQLite
                   /// sqINTEGER 64-bit signed integer (=SQLITE_INTEGER)
                   sqINTEGER = 1,
                   /// sqDOUBLE 64-bit IEEE floating point number (=SQLITE_FLOAT)
                   sqDOUBLE  = 2,
                   /// sqTEXT text string (=SQLITE_TEXT)
                   sqTEXT    = 3,
                   /// sqBLOB binary large object (=SQLITE_BLOB)
                   sqBLOB    = 4,
                   /// sqNULL no type  (=SQLITE_NULL)
                   sqNULL    = 5,
                   // === sqBLOB extensions follow =====
                   /// sqBLOB_PACKED structured data (e.g. containers) stored in packed blob form
                   sqBLOB_PACKED = 6,
                   /// sqEND end of type enums
                   sqEND
                };

/// low level: sqColumn represents a single column in a table
class OP_LITE_PUBLIC sqColumn {
   friend class sqTable;
public:

   /// default constructor
   sqColumn();

   /// copy constructor
   /*!  \param  name  name of column
        \param  pk    sqPK enum value indicating primary key status */
   sqColumn(const string& name, sqPK pk);

   /// destructor
   virtual ~sqColumn();

   /// clone the definition of this column
   /*! \return cloned copy of this column   */
   virtual sqColumn* clone() const = 0;

   /// return concrete value type as sqValueType enum
   /*! \return sqValueType enum   */
   virtual sqValueType type() const = 0;

   /// return concrete value type as string
   /*! \return concrete value type as string   */
   virtual string type_string() const = 0;

   /// return column name
   /*! \return column name   */
   const string& colnam() const;

   /// return primary key info for this column
   /*! \return primary key info as sqPK enum   */
   sqPK pk() const;

   /// check whether colmn contains pointer type
   /*! \return true if the name indicates a pointer type   */
   bool is_pointer_type() const;

   /// Check compatibility of 2 column objects
   /*! \return true if other column has the same column name as this   */
   bool is_compatible(const sqColumn* other) const;

   /// Return object pointed to (if any)
   /*! \return Return object pointed to, 0 if no pointer type   */
   virtual op_object* get_unique() { return 0; }

private:
   /// \privatesection (hidden in documentation)

   typedef set<string> StringSet;
   typedef StringSet::iterator  string_i;

   // a helper function to obtain a StringSet iterator for a string (the string will be inserted into the set if not already there)
   static string_i name_ref(const string& name);

   /// return value type as low level SQLITE_XXXX value
   virtual int sqlite3_type() const = 0;

private:

   // we keep a set of unique names, so that we don't have to duplicate the column name for every instance of the object type
   static StringSet m_names;

   string_i m_name;   // sqlite3 column name (as iterator referring to m_names)
   sqPK     m_pk;     // primary key info
};

typedef vector<sqColumn*> op_values;

#endif // SQCOLUMN_H
