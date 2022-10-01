#pragma once
/*
* The example bellow was extracted from https://gcc.gnu.org/wiki/Visibility
*/

// Generic helper definitions for shared library support
#if defined _WIN32 || defined __CYGWIN__
#ifndef CREATE_DLL
#define CAMERA_LIB_IMPORT __declspec(dllimport)
#else
#define CAMERA_LIB_EXPORT __declspec(dllexport)
#endif
#define CAMERA_LIB_LOCAL
#else
#if __GNUC__ >= 4
#define CAMERA_LIB_IMPORT __attribute__ ((visibility ("default")))
#define CAMERA_LIB_EXPORT __attribute__ ((visibility ("default")))
#define CAMERA_LIB_LOCAL  __attribute__ ((visibility ("hidden")))
#else
#define CAMERA_LIB_IMPORT
#define CAMERA_LIB_EXPORT
#define CAMERA_LIB_LOCAL
#endif
#endif

// Now we use the generic helper definitions above to define CAMERA_API and CAMERA_LOCAL.
// CAMERA_API is used for the public API symbols. It either DLL imports or DLL exports (or does nothing for static build)
// CAMERA_LOCAL is used for non-api symbols.

#ifdef CAMERA_DLL // defined if LIBRARY is compiled as a DLL
#ifdef CAMERA_LIB_EXPORT // defined if we are building the LIBRARY DLL (instead of using it)
#define CAMERA_API CAMERA_LIB_EXPORT
#else
#define CAMERA_API CAMERA_LIB_IMPORT
#endif
#define CAMERA_LOCAL CAMERA_LIB_LOCAL
#else // LIBRARY_DLL is not defined: this means LIBRARY is a static lib.
#define CAMERA_API
#define CAMERA_LOCAL
#endif