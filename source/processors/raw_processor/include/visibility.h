#pragma once
/*
* The example bellow was extracted from https://gcc.gnu.org/wiki/Visibility
*/

#// Generic helper definitions for shared library support
#if defined _WIN32 || defined __CYGWIN__
#ifndef CREATE_DLL
#define DUMMY_LIB_IMPORT __declspec(dllimport)
#else
#define DUMMY_LIB_EXPORT __declspec(dllexport)
#endif
#define DUMMY_LIB_LOCAL
#else
#if __GNUC__ >= 4
#define DUMMY_LIB_IMPORT __attribute__ ((visibility ("default")))
#define DUMMY_LIB_EXPORT __attribute__ ((visibility ("default")))
#define DUMMY_LIB_LOCAL  __attribute__ ((visibility ("hidden")))
#else
#define DUMMY_LIB_IMPORT
#define DUMMY_LIB_EXPORT
#define DUMMY_LIB_LOCAL
#endif
#endif

// Now we use the generic helper definitions above to define DUMMY_API and DUMMY_LOCAL.
// DUMMY_API is used for the public API symbols. It either DLL imports or DLL exports (or does nothing for static build)
// DUMMY_LOCAL is used for non-api symbols.

#ifdef DUMMY_DLL // defined if LIBRARY is compiled as a DLL
#ifdef DUMMY_LIB_EXPORT // defined if we are building the LIBRARY DLL (instead of using it)
#define DUMMY_API DUMMY_LIB_EXPORT
#else
#define DUMMY_API DUMMY_LIB_IMPORT
#endif
#define DUMMY_LOCAL DUMMY_LIB_LOCAL
#else // LIBRARY_DLL is not defined: this means LIBRARY is a static lib.
#define DUMMY_API
#define DUMMY_LOCAL
#endif