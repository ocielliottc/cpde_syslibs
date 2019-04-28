#ifndef OP_PACKED_H
#define OP_PACKED_H

#include "op_lite_config.h"
#include "sqlite3/sqlite3.h"
#include "sqColumn.h"
#include "sqBlob.h"
#include <vector>
using namespace std;
#include "op_msgpack.h"

/// The op_packed interface is base for persistent containers using msgpack persistence
class OP_LITE_PUBLIC op_packed : public sqColumn {
public:
   op_packed(const string& colnam) : sqColumn(colnam,sqPK_NONE)  {}
   virtual ~op_packed() {}

   /// return concrete value type as sqValueType enum
   /*! \return sqValueType enum   */
   sqValueType type() const                          { return sqBLOB_PACKED; }

   /// return concrete value type as string
   /*! \return concrete value type as string   */
   string type_string() const                        { return "BLOB"; }

   /// return concrete value type as SQLite type
   /*! \return concrete value type as SQLite type   */
   int sqlite3_type() const                          { return SQLITE_BLOB; }

   /// Return compression type used with containers
   virtual int ctyp() const                          { return m_ctyp; }

   /// pack the container into a blob
   virtual void pack(sqBlob& blob) = 0;

   /// unpack the container from given blob
   virtual void unpack(const sqBlob& blob) = 0;

   /// override default compression
   static void set_default_compression(int ctyp)     { m_ctyp = ctyp; }
protected:
   static int m_ctyp;  // compresson type used when writing
};

#endif // OP_PACKED_H
