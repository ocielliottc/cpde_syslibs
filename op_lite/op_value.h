#ifndef OP_VALUE_H
#define OP_VALUE_H

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

#include <vector>
#include "sqValue.h"
#include "sqRef.h"
#include "sqTable.h"
class op_object;

/// An op_value<T> is a base class template for column values
template<typename T>
class op_value : public sqValue<T> {
public:
   op_value(const string& colnam ) : sqValue<T>(colnam) {}
   op_value(const string& colnam, sqPK pk): sqValue<T>(colnam,pk) {}
   op_value(const string& colnam, const T& val) : sqValue<T>(colnam,val) {}
   op_value(const string& colnam, const T& val, sqPK pk) : sqValue<T>(colnam,val,pk) {}
   op_value<T>& operator=(const T& val) { sqValue<T>::set_value(val); return *this; }
   void set_value(const T& val) { sqValue<T>::set_value(val); }
   const T& value() const { return sqValue<T>::value(); }
   operator const T& () const { return sqValue<T>::value(); }
   virtual ~op_value() {}
};

typedef op_value<IDint64>  op_int;      /**< persistent integer value */
typedef op_value<double>   op_double;   /**< persistent double value */
typedef op_value<string>   op_string;   /**< persistent string value */
typedef op_value<sqBlob>   op_blob;     /**< persistent BLOB value */
typedef sqRef              op_pid;      /**< persistent object identifier value */

/*!
   \def op_construct(x)
   construct member variable so that column name = variable name \a x
*/
/*!
   \def op_construct_v1(x,v)
   construct member variable so that column name = variable name \a x and set initial value \a v
*/
/*!
   \def op_construct_pk(x)
   construct member variable so that column name = variable name \a x and also primary key
*/
/*!
   \def op_construct_pk_v1(x,v)
   construct member variable so that column name = variable name \a x , primary key and set initial value \a v
*/
/*!
   \def op_bind(x,y)
   bind the member variable \a y to the op_values \a x
*/


#define op_construct(x) x(#x)
#define op_construct_v1(x,v) x(#x,v)
#define op_construct_pk(x) x(#x,sqPK_1)
#define op_construct_pk_v1(x,v) x(#x,v,sqPK_1)
#define op_bind(x,y) (x).push_back(&y)

#endif // OP_VALUE_H
