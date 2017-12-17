#include "op_miniz.h"

// miniz C source file is included as a header, see miniz example1
#include "miniz.c"


// compress from inBuf to outBuf. On return outBuf has exact compressed size
bool op_miniz::Compress1(std::vector<unsigned char>& outBuf, const unsigned char* inBuf, size_t inSize)
{
   uLong src_len = (uLong)inSize;
   uLong cmp_len = compressBound(src_len);
   outBuf.resize(cmp_len);

   int cmp_status = compress(&outBuf[0], &cmp_len,inBuf, src_len);
   if (cmp_status != Z_OK) {
      outBuf.clear();
      return false;
   }

   outBuf.resize(cmp_len);
   return true;
}

// uncompress from inBuf to outBuf. unc_size is the known uncompressed size
bool op_miniz::Uncompress1(std::vector<unsigned char>& outBuf, unsigned unc_size, const unsigned char* inBuf, size_t inSize)
{
   outBuf.resize(unc_size);
   uLong uncomp_len = unc_size;
   int cmp_status = uncompress(&outBuf[0], &uncomp_len, inBuf, inSize);
   if (cmp_status != Z_OK) {
      outBuf.clear();
      return false;
   }
   outBuf.resize(uncomp_len);
   return true;
}
