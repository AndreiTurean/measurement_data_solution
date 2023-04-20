#pragma once
#include <cstdint>
#include <memory>
#include <cstring>

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
    /*!
    *   @brief Data package destructor. Delete the payload if it exists using the delete[] operator.
    */
    ~DataPackage()
    {
        if (payload)
        {
            delete[] reinterpret_cast<char*>(payload);
        }
    }

    /*!
    *   @brief Data package equal operator.
    *   @param lhs left hand side of the operator.
    *   @param rhs right hand side of the operator.
    *   @return true if the two packages are equal, false otherwise.
    */
    friend bool operator == (const DataPackage& lhs, const DataPackage& rhs)
    {
        return lhs.timestamp == rhs.timestamp &&
            lhs.sourceHandle == rhs.sourceHandle &&
            lhs.size == rhs.size &&
            lhs.cycle_ == rhs.cycle_ &&
            lhs.type == rhs.type &&
            std::memcmp(lhs.payload, rhs.payload, lhs.size) == 0;
    }

    /*!
    *   @brief Data package not equal operator.
    *   @param lhs left hand side of the operator.
    *   @param rhs right hand side of the operator.
    *   @return true if the two packages are not equal, false otherwise.
    */
    friend bool operator != (const DataPackage& lhs, const DataPackage& rhs)
    {
        return !(lhs == rhs);
    }

    /*!
    *   @brief Data package less than operator.
    *   @param lhs left hand side of the operator.
    *   @param rhs right hand side of the operator.
    *   @return true if the left hand side package is less than the right hand side package, false otherwise.
    */
    friend bool operator < (const DataPackage& lhs, const DataPackage& rhs)
    {
        return lhs.timestamp < rhs.timestamp;
    }

    /*!
    *   @brief Data package greater than operator.
    *   @param lhs left hand side of the operator.
    *   @param rhs right hand side of the operator.
    *   @return true if the left hand side package is greater than the right hand side package, false otherwise.
    */
    friend bool operator > (const DataPackage& lhs, const DataPackage& rhs)
    {
        return rhs < lhs;
    }

    /*!
    *   @brief Data package equal operator for smart pointers.
    *   @param lhs left hand side of the operator.
    *   @param rhs right hand side of the operator.
    *   @return true if the two packages are equal, false otherwise.
    */
    friend bool operator == (const std::shared_ptr<DataPackage>& lhs, const std::shared_ptr<DataPackage>& rhs)
    {
        return *lhs == *rhs;
    }

    /*!
    *   @brief Data package not equal operator for smart pointers.
    *   @param lhs left hand side of the operator.
    *   @param rhs right hand side of the operator.
    *   @return true if the two packages are not equal, false otherwise.
    */
    friend bool operator != (const std::shared_ptr<DataPackage>& lhs, const std::shared_ptr<DataPackage>& rhs)
    {
        return !(lhs == rhs);
    }

    /*!
    *   @brief Data package less than operator for smart pointers.
    *   @param lhs left hand side of the operator.
    *   @param rhs right hand side of the operator.
    *   @return true if the left hand side package is less than the right hand side package, false otherwise.
    */
    friend bool operator < (const std::shared_ptr<DataPackage>& lhs, const std::shared_ptr<DataPackage>& rhs)
    {
        return *lhs < *rhs;
    }

    /*!
    *   @brief Data package greater than operator for smart pointers.
    *   @param lhs left hand side of the operator.
    *   @param rhs right hand side of the operator.
    *   @return true if the left hand side package is greater than the right hand side package, false otherwise.
    */
    friend bool operator > (const std::shared_ptr<DataPackage>& lhs, const std::shared_ptr<DataPackage>& rhs)
    {
        return *rhs < *lhs;
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

/*!
*   @brief Data chunk header definition
*/
struct DataChunckHeader
{
    size_t dataSize; //!< size of the data chunk
    void* dataPtr; //!< pointer to where the data chunk starts
};