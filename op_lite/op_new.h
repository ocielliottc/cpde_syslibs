#ifndef OP_NEW_H
#define OP_NEW_H

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

#include "op_database.h"
#include "op_manager.h"
#include <memory>
using namespace std;

/// The op_new template works as "new operator" for constructing new persistent objects with up to 10 constructor parameters
template <typename T>
class op_new {
public:
   op_new() {
      make_persistent( new T() );
   }

   template<typename A1>
   op_new(const A1& a1) {
      make_persistent( new T(a1) );
   }
   template<typename A1,typename A2>
   op_new(const A1& a1,const A2& a2) {
      make_persistent( new T(a1,a2) );
   }
   template<typename A1,typename A2,typename A3>
   op_new(const A1& a1,const A2& a2,const A3& a3) {
      make_persistent( new T(a1,a2,a3) );
   }
   template<typename A1,typename A2,typename A3,typename A4>
   op_new(const A1& a1,const A2& a2,const A3& a3,const A4& a4) {
      make_persistent( new T(a1,a2,a3,a4) );
   }
   template<typename A1,typename A2,typename A3,typename A4,typename A5>
   op_new(const A1& a1,const A2& a2,const A3& a3,const A4& a4,const A5& a5) {
      make_persistent( new T(a1,a2,a3,a4,a5) );
   }
   template<typename A1,typename A2,typename A3,typename A4,typename A5,typename A6>
   op_new(const A1& a1,const A2& a2,const A3& a3,const A4& a4,const A5& a5,const A6& a6) {
      make_persistent( new T(a1,a2,a3,a4,a5,a6) );
   }
   template<typename A1,typename A2,typename A3,typename A4,typename A5,typename A6,typename A7>
   op_new(const A1& a1,const A2& a2,const A3& a3,const A4& a4,const A5& a5,const A6& a6,const A7& a7) {
      make_persistent( new T(a1,a2,a3,a4,a5,a6,a7) );
   }
   template<typename A1,typename A2,typename A3,typename A4,typename A5,typename A6,typename A7,typename A8>
   op_new(const A1& a1,const A2& a2,const A3& a3,const A4& a4,const A5& a5,const A6& a6,const A7& a7,const A8& a8) {
      make_persistent( new T(a1,a2,a3,a4,a5,a6,a7,a8) );
   }
   template<typename A1,typename A2,typename A3,typename A4,typename A5,typename A6,typename A7,typename A8,typename A9>
   op_new(const A1& a1,const A2& a2,const A3& a3,const A4& a4,const A5& a5,const A6& a6,const A7& a7,const A8& a8,const A9& a9) {
      make_persistent( new T(a1,a2,a3,a4,a5,a6,a7,a8,a9) );
   }
   template<typename A1,typename A2,typename A3,typename A4,typename A5,typename A6,typename A7,typename A8,typename A9,typename A10>
   op_new(const A1& a1,const A2& a2,const A3& a3,const A4& a4,const A5& a5,const A6& a6,const A7& a7,const A8& a8,const A9& a9,const A10& a10) {
      make_persistent( new T(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10) );
   }

   operator T*() const {return m_object;}
   T* operator()() const {return m_object;}

private:
   void make_persistent(T* object)
   {
      m_object = 0;
      op_database* db = op_mgr()->selected_database();
      if(!db) {
         string msg = typeid(this).name();
         msg += ", selected database is null";
         throw logic_error(msg);
      }
      if(db->make_persistent(object)) {
         m_object = object;
      }
      else delete object;
   }

private:
   T* m_object;
};

/// The op_new_db template works the same as op_new, but does not rely on a currently selected database
template <typename T>
class op_new_db {
public:
   op_new_db(op_database* db) {
     make_persistent(db, new T() );
   }
   template<typename A1>
   op_new_db(op_database* db, const A1& a1) {
      make_persistent(db, new T(a1) );
   }
   template<typename A1,typename A2>
   op_new_db(op_database* db, const A1& a1,const A2& a2) {
      make_persistent(db, new T(a1,a2) );
   }
   template<typename A1,typename A2,typename A3>
   op_new_db(op_database* db, const A1& a1,const A2& a2,const A3& a3) {
      make_persistent(db, new T(a1,a2,a3) );
   }
   template<typename A1,typename A2,typename A3,typename A4>
   op_new_db(op_database* db, const A1& a1,const A2& a2,const A3& a3,const A4& a4) {
      make_persistent(db, new T(a1,a2,a3,a4) );
   }
   template<typename A1,typename A2,typename A3,typename A4,typename A5>
   op_new_db(op_database* db, const A1& a1,const A2& a2,const A3& a3,const A4& a4,const A5& a5) {
      make_persistent(db, new T(a1,a2,a3,a4,a5) );
   }
   template<typename A1,typename A2,typename A3,typename A4,typename A5,typename A6>
   op_new_db(op_database* db, const A1& a1,const A2& a2,const A3& a3,const A4& a4,const A5& a5,const A6& a6) {
      make_persistent(db, new T(a1,a2,a3,a4,a5,a6) );
   }
   template<typename A1,typename A2,typename A3,typename A4,typename A5,typename A6,typename A7>
   op_new_db(op_database* db, const A1& a1,const A2& a2,const A3& a3,const A4& a4,const A5& a5,const A6& a6,const A7& a7) {
      make_persistent(db, new T(a1,a2,a3,a4,a5,a6,a7) );
   }
   template<typename A1,typename A2,typename A3,typename A4,typename A5,typename A6,typename A7,typename A8>
   op_new_db(op_database* db, const A1& a1,const A2& a2,const A3& a3,const A4& a4,const A5& a5,const A6& a6,const A7& a7,const A8& a8) {
      make_persistent(db, new T(a1,a2,a3,a4,a5,a6,a7,a8) );
   }
   template<typename A1,typename A2,typename A3,typename A4,typename A5,typename A6,typename A7,typename A8,typename A9>
   op_new_db(op_database* db, const A1& a1,const A2& a2,const A3& a3,const A4& a4,const A5& a5,const A6& a6,const A7& a7,const A8& a8,const A9& a9) {
      make_persistent(db, new T(a1,a2,a3,a4,a5,a6,a7,a8,a9) );
   }
   template<typename A1,typename A2,typename A3,typename A4,typename A5,typename A6,typename A7,typename A8,typename A9,typename A10>
   op_new_db(op_database* db, const A1& a1,const A2& a2,const A3& a3,const A4& a4,const A5& a5,const A6& a6,const A7& a7,const A8& a8,const A9& a9,const A10& a10) {
      make_persistent(db, new T(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10) );
   }

   operator T*() const {return m_object;}
   T* operator()() const {return m_object;}

private:
   void make_persistent(op_database* db, T* object)
   {
      m_object = 0;
      if(!db) {
         string msg = typeid(this).name();
         msg += ", given database is null";
         throw logic_error(msg);
      }
      if(db->make_persistent(object)) {
         m_object = object;
      }
      else delete object;
   }

private:
   T* m_object;
};



/// op_delete: a kind of delete operator for deleting persistent objects from database and transient memory
OP_LITE_PUBLIC
void op_delete(op_object* object);

#endif // OP_NEW_H
