#pragma once
/*
* The example bellow was extracted from https://gcc.gnu.org/wiki/Visibility
*/

#// Generic helper definitions for shared library support
#if defined _WIN32 || defined __CYGWIN__
#ifndef CREATE_DLL
#define REPLAY_LIB_IMPORT __declspec(dllimport)
#else
#define REPLAY_LIB_EXPORT __declspec(dllexport)
#endif
#define ASC_READER_LIB_LOCAL
#else
#if __GNUC__ >= 4
#define ASC_READER_LIB_IMPORT __attribute__ ((visibility ("default")))
#define ASC_READER_LIB_EXPORT __attribute__ ((visibility ("default")))
#define ASC_READER_LIB_LOCAL  __attribute__ ((visibility ("hidden")))
#else
#define ASC_READER_LIB_IMPORT
#define ASC_READER_LIB_EXPORT
#define ASC_READER_LIB_LOCAL
#endif
#endif

// Now we use the generic helper definitions above to define ASC_READER_API and ASC_READER_LOCAL.
// ASC_READER_API is used for the public API symbols. It either DLL imports or DLL exports (or does nothing for static build)
// ASC_READER_LOCAL is used for non-api symbols.

#ifdef ASC_READER_DLL // defined if LIBRARY is compiled as a DLL
#ifdef ASC_READER_LIB_EXPORT // defined if we are building the LIBRARY DLL (instead of using it)
#define ASC_READER_API ASC_READER_LIB_EXPORT
#else
#define ASC_READER_API ASC_READER_LIB_IMPORT
#endif
#define ASC_READER_LOCAL ASC_READER_LIB_LOCAL
#else // LIBRARY_DLL is not defined: this means LIBRARY is a static lib.
#define ASC_READER_API
#define ASC_READER_LOCAL
#endif