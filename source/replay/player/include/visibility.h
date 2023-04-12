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
#define REPLAY_LIB_LOCAL
#else
#if __GNUC__ >= 4
#define REPLAY_LIB_IMPORT __attribute__ ((visibility ("default")))
#define REPLAY_LIB_EXPORT __attribute__ ((visibility ("default")))
#define REPLAY_LIB_LOCAL  __attribute__ ((visibility ("hidden")))
#else
#define REPLAY_LIB_IMPORT
#define REPLAY_LIB_EXPORT
#define REPLAY_LIB_LOCAL
#endif
#endif

// Now we use the generic helper definitions above to define REPLAY_API and REPLAY_LOCAL.
// REPLAY_API is used for the public API symbols. It either DLL imports or DLL exports (or does nothing for static build)
// REPLAY_LOCAL is used for non-api symbols.

#ifdef REPLAY_DLL // defined if LIBRARY is compiled as a DLL
#ifdef REPLAY_LIB_EXPORT // defined if we are building the LIBRARY DLL (instead of using it)
#define REPLAY_API REPLAY_LIB_EXPORT
#else
#define REPLAY_API REPLAY_LIB_IMPORT
#endif
#define REPLAY_LOCAL REPLAY_LIB_LOCAL
#else // LIBRARY_DLL is not defined: this means LIBRARY is a static lib.
#define REPLAY_API
#define REPLAY_LOCAL
#endif