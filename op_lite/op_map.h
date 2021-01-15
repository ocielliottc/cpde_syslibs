#ifndef OP_MAP_H
#define OP_MAP_H

#include "op_packed.h"
#include <map>
using namespace std;

/// The op_map is an associative persistent container similar to std::map
template <typename K, typename V>
class op_map : public op_packed {
public:
   typedef typename map<K,V>::iterator iterator;
   typedef typename map<K,V>::const_iterator const_iterator;
   typedef K key_type;
   typedef pair<K,V> value_type;
   typedef V mapped_type;

public:
   /// constructor used via op_construct() to initialize persistent map member
   /*! \param colnam column name of map */
   op_map(const string& colnam ) : op_packed(colnam) {}

   /// constructor used via op_construct_v1() to initialize persistent map member
   /*! \param colnam column name of map
       \param amap std::map containing initial values */
   op_map(const string& colnam, const map<K,V>& amap ) : op_packed(colnam),m_map(amap) {}

   /// destructor
   virtual ~op_map() {}

   /// Clear the contents
   void clear()                                        { m_map.clear(); }

   /// return the size of the map
   size_t size() const                                 { return m_map.size(); }

   /// return first iterator
   iterator begin()                                    { return m_map.begin(); }

   // return first const_iterator
   const_iterator begin() const                        { return m_map.begin(); }

   /// return one beyond last iterator
   iterator end()                                      { return m_map.end(); }

   /// return one beyond last const_iterator
   const_iterator end() const                          { return m_map.end(); }

   /// insert value into map (i.e. a pair)
   /*! \param val value to insert into map */
   pair<iterator,bool> insert(const value_type& val)   { return m_map.insert(val); }

   /// reference a single map element, it will be created if not already existing
   /*! \param k key value identifying map entry */
   mapped_type& operator[](const key_type& k)          { return m_map[k]; }

   /// Searches the container for an element with a key equivalent to k and returns an iterator to it if found
   /*! \param k key value identifying map entry */
   iterator find(const key_type& k)                    { return m_map.find(k); }

   /// Searches the container for an element with a key equivalent to k and returns a const_iterator to it if found
   /*! \param k key value identifying map entry */
   const_iterator find(const key_type& k) const        { return m_map.find(k); }

   /// erase element identified by iterator
   /*! \param position iterator identifying entry to be erased */
   void   erase(iterator position)                     { m_map.erase(position); }

   /// erase element identified by key
   /*! \param k key value identifying entry to be erased */
   size_t erase(const key_type& k)                     { return m_map.erase(k); }

   /// special function to return read access to "native" std::map member
   const map<K,V>& native() const                      { return m_map; }

protected:
   /// \privatesection (hidden in documentation)

   /// clone the definition of this column
   /*! \return cloned copy of this column   */
   sqColumn* clone() const { return new op_map<K,V>(colnam()); }

   /// pack the container into a blob
   void pack(sqBlob& blob);

   /// unpack the container from blob
   void unpack(const sqBlob& blob);

private:
   map<K,V>  m_map;
};


template <typename K, typename V>
void op_map<K,V>::pack(sqBlob& blob)
{
   // pack the container into blob

   // first pack into message buffer
   msgpack::sbuffer sbuf;
   msgpack::pack(&sbuf,m_map);

   // then compress the message buffer into a blob
   blob.pack_blob(ctyp(),(const unsigned char*)sbuf.data(),sbuf.size());
}

template <typename K, typename V>
void op_map<K,V>::unpack(const sqBlob& blob)
{
   m_map.clear();
   if(blob.size_bytes() > 0) {

      // get_vector ensures that the blob data is uncompressed to a buffer
      std::vector<unsigned char> vec;
      blob.get_vector(vec);

      // then unpack the message buffer

  ///  old version
  ///  msgpack::unpacked msg;    // includes memory pool and deserialized object
  ///  msgpack::unpack(&msg,(const char*)&vec[0],vec.size());
  ///  msgpack::object obj = msg.get();

  /// new version
      msgpack::object_handle oh = msgpack::unpack((const char*)&vec[0], vec.size());
      msgpack::object obj = oh.get();

      // finally reconstruct the map
      obj.convert(m_map);
   }
}


#endif // OP_MAP_H
