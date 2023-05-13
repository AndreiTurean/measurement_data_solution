#pragma once
/*
* The example bellow was extracted from https://gcc.gnu.org/wiki/Visibility
*/

#// Generic helper definitions for shared library support
#if defined _WIN32 || defined __CYGWIN__
#ifndef CREATE_DLL
#define PCAP_READER_LIB_IMPORT __declspec(dllimport)
#else
#define PCAP_READER_LIB_EXPORT __declspec(dllexport)
#endif
#define PCAP_READER_LIB_LOCAL
#else
#if __GNUC__ >= 4
#define PCAP_READER_LIB_IMPORT __attribute__ ((visibility ("default")))
#define PCAP_READER_LIB_EXPORT __attribute__ ((visibility ("default")))
#define PCAP_READER_LIB_LOCAL  __attribute__ ((visibility ("hidden")))
#else
#define PCAP_READER_LIB_IMPORT
#define PCAP_READER_LIB_EXPORT
#define PCAP_READER_LIB_LOCAL
#endif
#endif

// Now we use the generic helper definitions above to define PCAP_READER_API and PCAP_READER_LOCAL.
// PCAP_READER_API is used for the public API symbols. It either DLL imports or DLL exports (or does nothing for static build)
// PCAP_READER_LOCAL is used for non-api symbols.

#ifdef PCAP_READER_DLL // defined if LIBRARY is compiled as a DLL
#ifdef PCAP_READER_LIB_EXPORT // defined if we are building the LIBRARY DLL (instead of using it)
#define PCAP_READER_API PCAP_READER_LIB_EXPORT
#else
#define PCAP_READER_API PCAP_READER_LIB_IMPORT
#endif
#define PCAP_READER_LOCAL PCAP_READER_LIB_LOCAL
#else // LIBRARY_DLL is not defined: this means LIBRARY is a static lib.
#define PCAP_READER_API
#define PCAP_READER_LOCAL
#endif