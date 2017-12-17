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
#include "op_new.h"
#include "op_object.h"

void op_delete(op_object* object)
{
   if(op_database* db = object->db()) {
      db->delete_persistent(object);
   }
   else {
      delete object;
      object = 0;
   }
}

