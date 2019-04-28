#ifndef OP_MSGPACK_H
#define OP_MSGPACK_H

// see https://github.com/msgpack/msgpack-c

// MessagePack is about creating short representations.
// The warning about truncating constant must be removed to avoid excessive warnings
// in op_lite applications when compiling with MSVC compiler.

#ifdef _MSC_VER
   #pragma warning(push)
   #pragma warning(disable: 4309) // warning C4309: 'static_cast' : truncation of constant value
#endif

#include <msgpack.hpp>

#ifdef _MSC_VER
   #pragma warning(pop)
#endif

#endif // OP_MSGPACK_H
