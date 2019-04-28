#ifndef OP_MINIZ_H
#define OP_MINIZ_H

#include <vector>
#include <stddef.h>

// op_miniz is usinu miniz as alternative compression method (compared to to e.g. lzma)
// but is provided here with a similar interface

class op_miniz {
public:
   // compress from inBuf to outBuf. On return outBuf has exact compressed size
   static bool Compress1(std::vector<unsigned char>& outBuf, const unsigned char* inBuf, size_t inSize);

   // uncompress from inBuf to outBuf. unc_size is the known uncompressed size
   static bool Uncompress1(std::vector<unsigned char>& outBuf, unsigned unc_size, const unsigned char* inBuf, size_t inSize);
};

#endif // OP_MINIZ_H
