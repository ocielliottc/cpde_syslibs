// BeginLicense:
// Part of: cpde_syslibs - Cross Platform Development Environment, system libraries
// Copyright (C) 2017 Carsten Arnholm
// All rights reserved
//
// Permission is granted to anyone to use this software for any
// purpose, including commercial applications, and to alter it and
// redistribute it freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you
//    must not claim that you wrote the original software. If you use
//    this software in a product, an acknowledgment in the product
//    documentation would be appreciated.
//
// 2. Altered source versions must be plainly marked as such, and
//    must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
// This file is provided "AS IS" with NO WARRANTY OF ANY KIND,
// INCLUDING THE WARRANTIES OF DESIGN, MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE.
// EndLicense:

#include "as_vector.h"
#include "as_factory.h"

CScriptArray* CreateScriptArrayEmpty(const std::string& type, size_t len)
{
   // Create the array object with proper length
   asIScriptEngine* engine    = asF()->engine();
   asITypeInfo* arrayType     = engine->GetTypeInfoById(engine->GetTypeIdByDecl(string("array<"+type+">").c_str()));
   CScriptArray* script_array = CScriptArray::Create(arrayType,(asUINT)len);
   script_array->AddRef();
   return script_array;
}
