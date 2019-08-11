#include "as_ostream.h"

static as_ostream as_ostream_cout;

as_ostream::as_ostream()
: as_reftype(as_typeid<as_ostream>())
{}

as_ostream::~as_ostream()
{}

void as_ostream::InstallType(asIScriptEngine* engine)
{
   const std::string type_name = as_typeid<as_ostream>();

   // register the type
   int r = engine->RegisterObjectType(type_name.c_str(), 0, asOBJ_REF); as_assert( r >= 0 );

   // Register the addref/release behaviours
   r = engine->RegisterObjectBehaviour(type_name.c_str(), asBEHAVE_ADDREF,  "void Addref()",  asMETHOD(as_reftype,Addref),  asCALL_THISCALL); as_assert( r >= 0 );
   r = engine->RegisterObjectBehaviour(type_name.c_str(), asBEHAVE_RELEASE, "void Release()", asMETHOD(as_reftype,Release), asCALL_THISCALL); as_assert( r >= 0 );

   // register the methods
   as_ostream::DeclareMethods(type_name,engine);

   // register the cout property
   r = engine->RegisterGlobalProperty( "as_ostream cout", &as_ostream_cout );

}



