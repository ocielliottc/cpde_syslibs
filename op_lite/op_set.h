#ifndef OP_SET_H
#define OP_SET_H

#include "op_packed.h"
#include <set>
using namespace std;

/// The op_set is a sorted persistent container similar to std::set
template <typename T>
class op_set : public op_packed {
public:
  typedef typename std::set<T>::iterator iterator;
  typedef typename std::set<T>::const_iterator const_iterator;

  /// constructor used via op_construct() to initialize persistent set member
  /*! \param colnam column name of set */
  op_set(const string& colnam) : op_packed(colnam) {}

  /// constructor used via op_construct_v1() to initialize persistent set member
  /*! \param colnam column name of set
      \param st std::set containing initial values */
  op_set(const string& colnam, const std::set<T>& st) : op_packed(colnam), m_set(st) {}

  /// destructor
  virtual ~op_set() {}

  /// Clear the contents of the set
  void clear() { m_set.clear(); }

  /// return the size of the set
  size_t size() const { return m_set.size(); }

  /// return first iterator
  iterator begin() { return m_set.begin(); }

  // return first const_iterator
  const_iterator begin() const { return m_set.begin(); }

  /// return one beyond last iterator
  iterator end() { return m_set.end(); }

  /// return one beyond last const_iterator
  const_iterator end() const { return m_set.end(); }

  /// insert value
  /*! \param val the value to be inserted */
  std::pair<iterator, bool> insert(const T& val) { return m_set.insert(val); }

  /// writable reference to a single set element
  /*! \param i set index. Must be less than size() */
  T& operator[](size_t i) { return m_set[i]; }

  /// read-only reference to a single set element
  /*! \param i set index. Must be less than size() */
  const T& operator[](size_t i) const { return m_set[i]; }

  /// resize the set
  /*! \param n   new size of set
      \param val default value for elements*/
  void resize(size_t n, const T& val = T()) { m_set.resize(n, val); }

  /// reserve space
  /*! \param n  number of set elements to reserve memory for. Does not change size() */
  void reserve(size_t n) { m_set.reserve(n); }

  /// special function to return read access to "native" std::set member
  const std::set<T>& native() const { return m_set; }

protected:
  /// \privatesection (hidden in documentation)

  /// clone the definition of this column
  /*! \return cloned copy of this column   */
  sqColumn* clone() const { return new op_set<T>(colnam()); }

  /// pack the container into a blob
  void pack(sqBlob& blob)
  {
    // pack the container into blob

    // first pack into message buffer
    msgpack::sbuffer sbuf;
    msgpack::pack(&sbuf, m_set);

    // then compress the message buffer into a blob
    blob.pack_blob(ctyp(), reinterpret_cast<const unsigned char*>(sbuf.data()), sbuf.size());
  }

  /// unpack the container from blob
  void unpack(const sqBlob& blob)
  {
    // unpack the container from blob
    m_set.clear();
    if (blob.size_bytes() > 0) {

      // get_vector ensures that the data is uncompressed
      std::vector<unsigned char> vec;
      blob.get_vector(vec);

      // then unpack the message
      const msgpack::object_handle oh = msgpack::unpack(reinterpret_cast<const char*>(&vec[0]), vec.size());
      const msgpack::object obj = oh.get();

      // finally reconstruct the set
      obj.convert(m_set);
    }
  }


private:
  std::set<T> m_set;
};

template <typename T>
bool operator<(const op_ptr<T>& lhs, const op_ptr<T>& rhs)
{
  return *lhs.get() < *rhs.get();
}

#endif // OP_SET_H
