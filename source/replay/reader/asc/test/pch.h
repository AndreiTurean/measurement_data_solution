#pragma once
//STL
#include <string>
#include <filesystem>

//GTest
#include <gtest/gtest.h>

//MDS
#include <defs/MdsInterface.hpp>
#include <utilis/LibUtility.hpp>
#include <defs/MeasurementObjectDefs.hpp>
#include <core/Engine.hpp>
#include <defs/DataPackage.hpp>

typedef ReaderIfc* createReader(InterfaceAccess*);

#if defined(_WIN32)

#if DEBUG
    static const std::string LIB_NAME = "asc_reader_debug.dll";
#else
    static const std::string LIB_NAME = "asc_reader.dll";
#endif

#elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))

#if DEBUG
    static const std::string LIB_NAME = "libasc_reader_debug.so";
#else
    static const std::string LIB_NAME = "libasc_reader.so";
#endif

#else
    #error "Undefined OS!"
#endif