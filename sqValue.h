#ifndef SQVALUE_H
#define SQVALUE_H

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

#include "sqlite3/sqlite3.h"
#include "sqBlob.h"
#include "sqColumn.h"


/// low level: sqValue Represents a typed value in a table row
template<typename T>
class sqValue : public sqColumn {
public:
   /// constructor
   /*! \param name column name */
   sqValue(const string& name)                                    : sqColumn(name,sqPK_NONE),m_value() {}


   /// constructor
   /*! \param name column name
       \param pk primary key status for this column */
   sqValue(const string& name, sqPK pk)                           : sqColumn(name,pk),m_value()        {}

   /// constructor
   /*! \param name column name
       \param value initial value of this column
       \param pk primary key status for this column */
   sqValue(const string& name, const T& value, sqPK pk=sqPK_NONE) : sqColumn(name,pk),m_value(value)   {}

   /// destructor
   ~sqValue() {}

   /// clone the column definition and value
   virtual sqColumn* clone() const { return new sqValue<T>(colnam(),m_value,pk()); }

   /// return value by const reference
   inline const T& value() const { return m_value; }

   /// change column value
   inline void set_value(const T& val) { m_value = val; }

   /// return value type as sqValueType enum
   virtual sqValueType type() const;

   /// return value type as string
   string type_string() const;

   /// return value type as low level SQLITE_XXXX value
   int sqlite3_type() const;

protected:
   T      m_value;   // sqlite3 value representation
};

// template specialisations for the basic types we allow
template<> inline sqValueType sqValue<IDint64>::type() const { return sqINTEGER; }
template<> inline sqValueType sqValue<double>::type()  const { return sqDOUBLE; }
template<> inline sqValueType sqValue<string>::type()  const { return sqTEXT; }
template<> inline sqValueType sqValue<sqBlob>::type()  const { return sqBLOB; }

template<> inline string sqValue<IDint64>::type_string() const { return "INTEGER"; }
template<> inline string sqValue<double>::type_string() const { return "REAL"; }
template<> inline string sqValue<string>::type_string() const { return "TEXT"; }
template<> inline string sqValue<sqBlob>::type_string() const { return "BLOB"; }

template<> inline int sqValue<IDint64>::sqlite3_type() const { return SQLITE_INTEGER; }
template<> inline int sqValue<double>::sqlite3_type()  const { return SQLITE_FLOAT; }
template<> inline int sqValue<string>::sqlite3_type()  const { return SQLITE_TEXT; }
template<> inline int sqValue<sqBlob>::sqlite3_type()  const { return SQLITE_BLOB; }

/// low level: sqValue<int> Represents a typed value in a table row, specialised for integer
template<>
class sqValue<int> :  public sqColumn {
public:
   sqValue(const string& name)                                          : sqColumn(name,sqPK_NONE),m_value(0) {}
   sqValue(const string& name, const int&     value, sqPK pk=sqPK_NONE) : sqColumn(name,pk), m_value(value) { m_intval = (int) m_value; }
   sqValue(const string& name, const IDint64& value, sqPK pk=sqPK_NONE) : sqColumn(name,pk), m_value(value) { m_intval = (int) m_value; }
   ~sqValue() {}

   // clone the definition, do not clone value
   sqColumn* clone() const { return new sqValue<int>(colnam(),0,pk()); }

   // return value by const reference
   inline const int& value() const  { m_intval = (int) m_value; return m_intval; }

   // change value
   inline void set_value(const int& val) { m_value = val; m_intval = val; }

   // return value type as sqValueType enum
   sqValueType type() const  { return sqINTEGER; }

   // return value type as string
   string type_string() const { return "INTEGER"; }

   // return value type as SQLITE_XXXX value
   int sqlite3_type() const  { return SQLITE_INTEGER; }

private:
   IDint64      m_value;    // sqlite3 value representation
   mutable int  m_intval;
};

#endif // SQVALUE_H
