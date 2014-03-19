#ifndef __KEXPORT_H
#define __KEXPORT_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined _WIN32 || defined __CYGWIN__
  #ifdef BUILDING_DLL
    #ifdef __GNUC__
      #define KLIB_EXPORT __attribute__ ((dllexport))
    #else
      #define KLIB_EXPORT __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #else
    #ifdef __GNUC__
      #define KLIB_EXPORT __attribute__ ((dllimport))
    #else
      #define KLIB_EXPORT __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #endif
  #define KLIB_INTERNAL
#else
  #if __GNUC__ >= 4
    #define KLIB_EXPORT __attribute__ ((visibility ("default")))
    #define KLIB_INTERNAL  __attribute__ ((visibility ("hidden")))
  #else
    #define KLIB_EXPORT
    #define KLIB_INTERNAL
  #endif
#endif

#ifdef __cplusplus
} //extern "C"
#endif

#endif //__KEXPORT_H
