/*
** Author: Carsten Arnholm, January 2010
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

#include <cstring>
#include "sqBlob.h"
#include <limits>
#include <stdexcept>

#include "lzmalib/op_lzma.h"
#include "miniz/op_miniz.h"
#include <iostream>
using namespace std;

sqBlob::Header::Header()
{
   size_t i=0;
   hval[i++] = sizeof(Header);  // offset to begining of user data
   hval[i++] = 0;               // compression type = 0 if data is uncompressed in SQL 1=lzma, 2=miniz
   hval[i++] = 0;               // uncompressed size of user data
   hval[i++] = 0;               // compressed size of user data (only if ctyp!=0)
}

sqBlob::Header::Header(size_t offset, size_t ctyp, size_t usiz, size_t csiz)
{
   size_t i=0;
   hval[i++] = offset;   // offset to begining of user data
   hval[i++] = ctyp;     // compression type = 0 if data is uncompressed in SQL, 1=lzma, 2=miniz
   hval[i++] = usiz;     // uncompressed size of user data
   hval[i++] = csiz;     // compressed size of user data (only if ctyp!=0)
}

sqBlob::Header::Header(IDint64 offset, IDint64 ctyp, IDint64 usiz, IDint64 csiz)
{
   size_t i=0;
   hval[i++] = offset;   // offset to begining of user data
   hval[i++] = ctyp;     // compression type = 0 if data is uncompressed in SQL 1=lzma, 2=miniz
   hval[i++] = usiz;     // uncompressed size of user data
   hval[i++] = csiz;     // compressed size of user data (only if ctyp!=0)
}

sqBlob::sqBlob()
{}

sqBlob::sqBlob(const unsigned char* data, size_t nbytes)
{
   // reconstruct the header
   const IDint64* hv = (const IDint64*) data;
   m_header = Header(hv[0],hv[1],hv[2],hv[3]);

   size_t ctyp = m_header.ctyp();

   // save raw header + data (may be compressed)
   m_data.resize(nbytes);
   memcpy(&m_data[0],data,nbytes);
}

sqBlob::sqBlob(const sqBlob& other)
: m_header(other.m_header)
, m_data(other.m_data)
{}

sqBlob& sqBlob::operator=(const sqBlob& other)
{
   m_header = other.m_header;
   m_data = other.m_data;

   return *this;
}

sqBlob::sqBlob(const unsigned char* data, size_t nbytes, size_t ctyp)
{
   // header will be assigned in pack_blob
   pack_blob(ctyp,data,nbytes);
}


sqBlob::~sqBlob()
{}

size_t sqBlob::size_bytes() const
{
   return m_header.usiz();
}

void sqBlob::pack_blob(size_t ctyp, const unsigned char* data, size_t nbytes)
{
  if(ctyp==0) {
      // no compression
      size_t usiz = nbytes;
      size_t csiz = 0;
      m_header = Header(sizeof(Header),ctyp,usiz,csiz);
      m_data.resize(m_header.hsiz()+usiz);
      memcpy(&m_data[0],m_header.hval ,m_header.hsiz()); // uncompressed header
      memcpy(&m_data[0]+m_header.hsiz(),data,usiz);      // uncompressed data
   }
   else {

      // use specified compression
      size_t usiz = nbytes;
      std::vector<unsigned char> cdata;
      switch(ctyp) {
         case 1: { op_miniz::Compress1(cdata,data,usiz); break; }
         case 2: {  op_lzma::Compress1(cdata,data,usiz); break; }
         default: { throw runtime_error("FATAL (sqBlob): unknown compression type."); }
      };

      size_t csiz = cdata.size();
      m_header = Header(sizeof(Header),ctyp,usiz,csiz);
      m_data.resize(m_header.hsiz()+csiz);
      memcpy(&m_data[0],m_header.hval ,m_header.hsiz());  // uncompressed header
      memcpy(&m_data[0]+m_header.hsiz(),&cdata[0],csiz);  // compressed data
   }
}

size_t sqBlob::unpack_blob(unsigned char* data, size_t max_bytes) const
{
   size_t copy_bytes = m_header.usiz();
   if(copy_bytes > max_bytes) {
      throw runtime_error("FATAL (sqBlob): unpack_blob, buffer too small.");
   }
   size_t ctyp   = m_header.ctyp();
   size_t offset = m_header.hsiz();
   if(ctyp == 0) {
      // no compression, skip the header in m_data
      memcpy(data,&m_data[0]+offset,copy_bytes);
      return copy_bytes;
   }
   else {

      // we have compressed data
      size_t csiz   = m_header.csiz();
      size_t usiz   = m_header.usiz();
      std::vector<unsigned char> udata(usiz);
      // use specified uncompress
      switch(ctyp) {
         case 1: { op_miniz::Uncompress1(udata,usiz,&m_data[0]+offset,csiz); break; }
         case 2: {  op_lzma::Uncompress1(udata,usiz,&m_data[0]+offset,csiz); break; }
         default: { throw runtime_error("FATAL (sqBlob): unknown compression type."); }
      };

      // copy out
      memcpy(data,&udata[0],udata.size());
      return udata.size();
   }
   return 0;
}

size_t  sqBlob::packed_size() const
{
   return m_data.size();
}

const unsigned char* sqBlob::data() const
{
   return &m_data[0];
}

