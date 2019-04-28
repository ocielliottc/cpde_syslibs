#ifndef OP_VECTOR_H
#define OP_VECTOR_H

#include "op_packed.h"
#include <vector>
using namespace std;

/// The op_vector is a sequential persistent container similar to std::vector
template <typename T>
class op_vector : public op_packed {
public:
   typedef typename vector<T>::iterator iterator;
   typedef typename vector<T>::const_iterator const_iterator;

   /// constructor used via op_construct() to initialize persistent vector member
   /*! \param colnam column name of vector */
   op_vector(const string& colnam ) : op_packed(colnam) {}

   /// constructor used via op_construct_v1() to initialize persistent vector member
   /*! \param colnam column name of vector
       \param vec std::vector containing initial values */
   op_vector(const string& colnam, const vector<T>& vec ) : op_packed(colnam),m_vec(vec) {}

   /// destructor
   virtual ~op_vector() {}

   /// Clear the contents of the vector
   void clear()                                { m_vec.clear(); }

   /// return the size of the vector
   size_t size() const                         { return m_vec.size(); }

   /// return first iterator
   iterator begin()                            { return m_vec.begin(); }

   // return first const_iterator
   const_iterator begin() const                { return m_vec.begin(); }

   /// return one beyond last iterator
   iterator end()                              { return m_vec.end(); }

   /// return one beyond last const_iterator
   const_iterator end() const                  { return m_vec.end(); }

   /// push value at the back of the vector
   /*! \param val the value to be pushed */
   void push_back(const T& val)                { m_vec.push_back(val); }

   /// writable reference to a single vector element
   /*! \param i vector index. Must be less than size() */
   T& operator[](size_t i)                     { return m_vec[i]; }

   /// read-only reference to a single vector element
   /*! \param i vector index. Must be less than size() */
   const T& operator[](size_t i) const         { return m_vec[i]; }

   /// resize the vector
   /*! \param n   new size of vector
       \param val default value for elements*/
   void resize(size_t n, const T& val = T())   { m_vec.resize(n,val); }

   /// reserve space
   /*! \param n  number of vector elements to reserve memory for. Does not change size() */
   void reserve(size_t n)                      { m_vec.reserve(n); }

   /// special function to return read access to "native" std::vector member
   const vector<T>& native() const             { return m_vec; }

protected:
   /// \privatesection (hidden in documentation)

   /// clone the definition of this column
   /*! \return cloned copy of this column   */
   sqColumn* clone() const { return new op_vector<T>(colnam()); }

   /// pack the container into a blob
   void pack(sqBlob& blob);

   /// unpack the container from blob
   void unpack(const sqBlob& blob);

private:
   vector<T> m_vec;
};


template <typename T>
void op_vector<T>::pack(sqBlob& blob)
{
   // pack the container into blob

   // first pack into message buffer
   msgpack::sbuffer sbuf;
   msgpack::pack(&sbuf,m_vec);

   // then compress the message buffer into a blob
   blob.pack_blob(ctyp(),(const unsigned char*)sbuf.data(),sbuf.size());
}

template <typename T>
void op_vector<T>::unpack(const sqBlob& blob)
{
   // unpack the container from blob
   m_vec.clear();
   if(blob.size_bytes() > 0) {

      // get_vector ensures that the data is uncompressed
      vector<unsigned char> vec;
      blob.get_vector(vec);

      // then unpack the message
   ///   msgpack::unpacked msg;    // includes memory pool and deserialized object
   ///   msgpack::unpack(&msg,(const char*)&vec[0],vec.size());
   ///   msgpack::object obj = msg.get();

  /// new version
      msgpack::object_handle oh = msgpack::unpack((const char*)&vec[0], vec.size());
      msgpack::object obj = oh.get();

      // finally reconstruct the vector
      obj.convert(m_vec);
   }
}



#endif // OP_VECTOR_H
