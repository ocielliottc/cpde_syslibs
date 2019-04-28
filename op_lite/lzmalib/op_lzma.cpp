#include "op_lzma.h"
#include <cassert>
#include <cstddef>
#include <stdexcept>
#include "LzmaLib.h"
// #include <fstream>
// #include <iomanip>


void op_lzma::Compress1(std::vector<unsigned char>& outBuf, const unsigned char* inBuf, size_t inSize)
{
   size_t propsSize = LZMA_PROPS_SIZE;
   size_t destLen = inSize*4;
   outBuf.resize(propsSize + destLen);

   int res = LzmaCompress(
            &outBuf[LZMA_PROPS_SIZE], &destLen,
            inBuf, inSize,
            &outBuf[0], &propsSize,
            -1, 1<<27, -1, -1, -1, -1, 2);

   assert(propsSize == LZMA_PROPS_SIZE);
   if(res == SZ_ERROR_MEM) {
/*
      std::ofstream out("SZ_ERROR_MEM.txt");
      out << inSize << std::endl;
      out << propsSize + destLen << std::endl;
      size_t ndouble = inSize*sizeof(unsigned char)/sizeof(double);
      out << ndouble << std::endl;
      double* buf = (double*)inBuf;
      for(size_t i=0;i<ndouble; i++) out << std::scientific <<  std::setprecision(12) << buf[i]  << std::endl;
*/
      throw std::logic_error("op_lzma::Compress1: Memory allocation error, consider using other or no compression");
   }
   assert(res != SZ_ERROR_PARAM);
   assert(res != SZ_ERROR_OUTPUT_EOF);
   assert(res != SZ_ERROR_THREAD);
   assert(res == SZ_OK);

   outBuf.resize(propsSize + destLen);
}

void op_lzma::Compress1(std::vector<unsigned char> &outBuf,  const std::vector<unsigned char> &inBuf)
{
   Compress1(outBuf,&inBuf[0],inBuf.size());
}

void op_lzma::Uncompress1(std::vector<unsigned char>& outBuf, unsigned unc_size, const unsigned char* inBuf, size_t inSize)
{
   outBuf.resize(unc_size);
   size_t dstLen = outBuf.size();
   size_t srcLen = inSize - LZMA_PROPS_SIZE;
   SRes res = LzmaUncompress(
            &outBuf[0], &dstLen,
            &inBuf[LZMA_PROPS_SIZE], &srcLen,
            &inBuf[0], LZMA_PROPS_SIZE);
            assert(res == SZ_OK);
   outBuf.resize(dstLen); // If uncompressed data can be smaller
}


void op_lzma::Uncompress1( std::vector<unsigned char> &outBuf, unsigned unc_size, const std::vector<unsigned char> &inBuf)
{
   Uncompress1(outBuf,unc_size,&inBuf[0],inBuf.size());
}
