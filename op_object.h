#ifndef OP_OBJECT_H
#define OP_OBJECT_H

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

#include "op_lite_config.h"
#include <string>
using namespace std;

class op_database;
#include "op_value.h"

/// The op_object class is the base class for all op_lite persistent objects
class OP_LITE_PUBLIC op_object {
public:
   op_object();
   virtual ~op_object();

   /// get persistence layout of this type
   /*! /param pvalues container of persistent member variables */
   virtual void op_layout(op_values& pvalues) = 0;

   /// flush object to database immediately
   virtual bool update_persistent();

   /// return object type identifier string
   virtual string op_type() const;

   /// return database of this object
   op_database* db() const;

   /// return the persistent id of this object
   const op_pid& pid() const;

   /// return true if this object is persistent
   bool is_persistent() const;

public:
   /// \privatesection (hidden in documentation)

   /// bind the object to the database
   void bind_persistent(const op_pid& refid);

private:
   op_pid  m_pid;    // the object's persistent identifier
};


#endif // OP_OBJECT_H
