#ifndef CF_LZMA1_H
#define CF_LZMA1_H

// see http://www.asawicki.info/news_1368_lzma_sdk_-_how_to_use.html

#include <vector>
#include <cstddef>

class op_lzma {
public:

   // ===== basic in-memory compression/uncompression

   // compress from inBuf to outBuf. On return outBuf has exact compressed size
   static void Compress1(std::vector<unsigned char>& outBuf, const unsigned char* inBuf, size_t inSize);

   // compress from inBuf to outBuf. On return outBuf has exact compressed size
   static void Compress1(std::vector<unsigned char>& outBuf, const std::vector<unsigned char>& inBuf);

   // uncompress from inBuf to outBuf. unc_size is the known uncompressed size
   static void Uncompress1(std::vector<unsigned char>& outBuf, unsigned unc_size, const unsigned char* inBuf, size_t inSize);

   // uncompress from inBuf to outBuf. unc_size is the known uncompressed size
   static void Uncompress1(std::vector<unsigned char>& outBuf, unsigned unc_size, const std::vector<unsigned char>& inBuf);


   // ===== helper functions for in-memory compression/uncompression when vector is of any fixed size type

   // compress from inBuf to outBuf. On return outBuf has exact compressed size
   template <typename T>
   static void Compress(std::vector<unsigned char>& outBuf, const std::vector<T>& inBuf);

   // uncompress from inBuf to outBuf. unc_size is the known uncompressed size of outBuf
   template <typename T>
   static void Uncompress(std::vector<T>& outBuf, size_t unc_size, const std::vector<unsigned char>& inBuf);
};

template <typename T>
void op_lzma::Compress(std::vector<unsigned char>& outBuf, const std::vector<T>& inBuf)
{
   // convert inBuf to unsigned char
   std::vector<unsigned char> in_buf;
   size_t in_bytes = inBuf.size()*sizeof(T);
   in_buf.resize(in_bytes);
   memcpy(&in_buf[0],&inBuf[0],in_bytes);

   // do the compression
   Compress1(outBuf,in_buf);
}

template <typename T>
void op_lzma::Uncompress(std::vector<T>& outBuf, size_t unc_size, const std::vector<unsigned char>& inBuf)
{
   std::vector<unsigned char> out_buf;
   size_t unc_size_bytes = unc_size*sizeof(T);
   out_buf.resize(unc_size_bytes);

   // do the uncompression
   Uncompress1(out_buf,unc_size_bytes,inBuf);

   // copy out
   outBuf.resize(unc_size);
   memcpy(&outBuf[0],&out_buf[0],unc_size_bytes);
}

#endif // CF_LZMA1_H
