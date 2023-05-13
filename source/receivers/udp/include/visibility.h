#pragma once
/*
* The example bellow was extracted from https://gcc.gnu.org/wiki/Visibility
*/

#// Generic helper definitions for shared library support
#if defined _WIN32 || defined __CYGWIN__
#ifndef CREATE_DLL
#define UDP_RECEIVER_LIB_IMPORT __declspec(dllimport)
#else
#define UDP_RECEIVER_LIB_EXPORT __declspec(dllexport)
#endif
#define UDP_RECEIVER_LIB_LOCAL
#else
#if __GNUC__ >= 4
#define UDP_RECEIVER_LIB_IMPORT __attribute__ ((visibility ("default")))
#define UDP_RECEIVER_LIB_EXPORT __attribute__ ((visibility ("default")))
#define UDP_RECEIVER_LIB_LOCAL  __attribute__ ((visibility ("hidden")))
#else
#define UDP_RECEIVER_LIB_IMPORT
#define UDP_RECEIVER_LIB_EXPORT
#define UDP_RECEIVER_LIB_LOCAL
#endif
#endif

// Now we use the generic helper definitions above to define UDP_RECEIVER_API and UDP_RECEIVER_LOCAL.
// UDP_RECEIVER_API is used for the public API symbols. It either DLL imports or DLL exports (or does nothing for static build)
// UDP_RECEIVER_LOCAL is used for non-api symbols.

#ifdef UDP_RECEIVER_DLL // defined if LIBRARY is compiled as a DLL
#ifdef UDP_RECEIVER_LIB_EXPORT // defined if we are building the LIBRARY DLL (instead of using it)
#define UDP_RECEIVER_API UDP_RECEIVER_LIB_EXPORT
#else
#define UDP_RECEIVER_API UDP_RECEIVER_LIB_IMPORT
#endif
#define UDP_RECEIVER_LOCAL UDP_RECEIVER_LIB_LOCAL
#else // LIBRARY_DLL is not defined: this means LIBRARY is a static lib.
#define UDP_RECEIVER_API
#define UDP_RECEIVER_LOCAL
#endif