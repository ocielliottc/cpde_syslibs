#ifndef OP_UNIQUE_PTR_H
#define OP_UNIQUE_PTR_H


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

#include "op_ptr.h"
class op_object;

/// An op_unique_ptr<T> is similar to op_ptr<T>, but implements unique ownership of object pointed to
template <typename T>
class op_unique_ptr : public op_ptr<T> {
public:
   /// construct from persistent object identifier
   op_unique_ptr(const op_pid& pid ) : op_ptr<T>(pid) {}

   /// construct from column name and persistent object identifier
   op_unique_ptr(const string& colnam, const op_pid& pid ) : op_ptr<T>(colnam, pid) {}

   /// construct from column name
   op_unique_ptr(const string& colnam ) : op_ptr<T>(colnam) {}

   /// construct from column name and pointer
   op_unique_ptr(const string& colnam, T* ptr ) : op_ptr<T>(colnam,ptr) {}

   /// destructor
   virtual ~op_unique_ptr() {}

   /// get unique object pointed to
   virtual op_object* get_unique() { return op_ptr<T>::get(); }
};

#endif // OP_UNIQUE_PTR_H
