#pragma once
#include <cstdint>
#include <memory>

enum class PackageType : uint8_t
{
    dummy = 0x00,
    camera = 0x01,
    can = 0x02,
    flexray = 0x04,
    ethernet = 0x08
};
/*!
*   @brief Data package definition
*/
struct DataPackage
{
    ~DataPackage()
    {
        if (payload)
        {
            delete[] reinterpret_cast<char*>(payload);
        }
    }
    uint64_t timestamp; //!< package timestamp
    uint64_t sourceHandle; //!< package source handle
    size_t size; //!< package size
    uint8_t cycle_; //!< package cycle
    PackageType type; //!< package type
    void* payload; //!< pointer to where the package payload starts.
};
//! Data package pointer
using DataPackagePtr = std::shared_ptr<DataPackage>;
//! Const data package pointer
using DataPackageCPtr = const DataPackagePtr;