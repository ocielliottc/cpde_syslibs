#ifndef OP_ROOT_H
#define OP_ROOT_H

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

#include "op_object.h"
#include "op_ptr.h"

/// An op_root<T> represents a root object in the database
template <typename T>
class op_root : public op_object {
public:
   op_root();

   /// construct new root object from root name and user object in currently selected database
   /*!  \param  root_name name of root object
        \param  root User defined root */
   op_root(const string& root_name, T* root);

   /// restore user object from root in currently selected database (may throw exception)
   /*!  \param  root_name name of root object
        \return pointer to restored user object when cuccessful */
   static T* restore(const string& root_name);

   /// return the name of the root object
   string root_name() const { return m_root_name; }

   /// return the persistent pointer as raw string value
   string pointer_value() const { return m_root_ptr.value(); }

   /// destructor
   virtual ~op_root() {}

   /// define the table name for the root objects
   string op_type() const { return op_type_static(); }

   /// define the table name as static function, for use during restore
   static string op_type_static() { return "op_root"; }

   /// define the root table layout
   void op_layout(op_values& pvalues);

   /// get pointer to user object from root object
   T* get();

private:
   op_string m_root_name;   // name of the root object
   op_ptr<T> m_root_ptr;    // pointer to actual object
};

template <typename T>
op_root<T>::op_root()
: op_construct_pk(m_root_name)
, op_construct(m_root_ptr)
{}

template <typename T>
op_root<T>::op_root(const string& root_name, T* root)
: op_construct_pk_v1(m_root_name,root_name)
, op_construct_v1(m_root_ptr,root)
{}

template <typename T>
void op_root<T>::op_layout(op_values& pvalues)
{
   op_bind(pvalues,m_root_name);
   op_bind(pvalues,m_root_ptr);
}

template <typename T>
T* op_root<T>::get()
{
   return m_root_ptr.operator->();
}

template <typename T>
T* op_root<T>::restore(const string& root_name)
{
   op_database* db = op_mgr()->selected_database();
   if(!db) {
      throw logic_error("op_root<T>::restore, no database selected.");
   }

   // look up the root name in the root table
   list<op_pid> ids;
   string where_clause = "where m_root_name = '" + root_name + "'";
   db->select_ids(ids,op_type_static(),where_clause);
   if(ids.size() == 1) {

      // ok, name found, restore as root object
      op_pid& pid = *ids.begin();
      op_root<T> root;
      if(db->restore_persistent(pid.id(),&root)) {

         // root restored, get user object
         T* object = root.get();
         if(!object) {
            throw logic_error("op_root<T>::restore, root " + root_name + " found, but '" + root.pointer_value() + "' could not be restored as '" + typeid(T).name() +"'." );
         }
         return object;
      }
      else {
         throw logic_error("op_root<T>::restore, root " + root_name + " found, but root object restore failed.");
      }
   }
   else {
      throw logic_error("op_root<T>::restore, no such name: " + root_name);
   }
   return 0;
}


#endif // OP_ROOT_H
