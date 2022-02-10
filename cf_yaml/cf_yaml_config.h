#ifndef CF_YAML_CONFIG_H_INCLUDED
#define CF_YAML_CONFIG_H_INCLUDED

#ifdef _MSC_VER

   /// MSVC compiler
   // disabling warning 4251 is the only practical way to deal with dll-export issues according to M$
   #pragma warning (disable:4251) // class "XX" needs to have dll-interface to be used by clients of class "YY"

   #ifdef CF_YAML_IMPLEMENTATION
      #define CF_YAML_PUBLIC __declspec(dllexport)
      #define CF_YAML_EXTERN
   #else
      #define CF_YAML_PUBLIC __declspec(dllimport)
      #define CF_YAML_EXTERN extern
   #endif
   #define CF_YAML_PRIVATE

#elif __GNUC__

   /// gnu gcc

   #define CF_YAML_PUBLIC  __attribute__ ((visibility("default")))
   #define CF_YAML_PRIVATE __attribute__ ((visibility("hidden")))
   #define CF_YAML_EXTERN

#else

  #error "Unknown compiler"

#endif


#endif // CF_YAML_CONFIG_H_INCLUDED

