#pragma once
/*
* The example bellow was extracted from https://gcc.gnu.org/wiki/Visibility
*/

#// Generic helper definitions for shared library support
#if defined _WIN32 || defined __CYGWIN__
#ifndef CREATE_DLL
#define USB_LIB_IMPORT __declspec(dllimport)
#else
#define USB_LIB_EXPORT __declspec(dllexport)
#endif
#define USB_LIB_LOCAL
#else
#if __GNUC__ >= 4
#define USB_LIB_IMPORT __attribute__ ((visibility ("default")))
#define USB_LIB_EXPORT __attribute__ ((visibility ("default")))
#define USB_LIB_LOCAL  __attribute__ ((visibility ("hidden")))
#else
#define USB_LIB_IMPORT
#define USB_LIB_EXPORT
#define USB_LIB_LOCAL
#endif
#endif

// Now we use the generic helper definitions above to define USB_API and USB_LOCAL.
// USB_API is used for the public API symbols. It either DLL imports or DLL exports (or does nothing for static build)
// USB_LOCAL is used for non-api symbols.

#ifdef USB_DLL // defined if LIBRARY is compiled as a DLL
#ifdef USB_LIB_EXPORT // defined if we are building the LIBRARY DLL (instead of using it)
#define USB_API USB_LIB_EXPORT
#else
#define USB_API USB_LIB_IMPORT
#endif
#define USB_LOCAL USB_LIB_LOCAL
#else // LIBRARY_DLL is not defined: this means LIBRARY is a static lib.
#define USB_API
#define USB_LOCAL
#endif