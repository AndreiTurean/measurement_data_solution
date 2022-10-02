#pragma once
#include <cstdint>
#include <defs/DataPackage.hpp>
/*!
*   @brief Data distribution interface. Let distribute a package to all linked the receivers.
*/
struct DataDistribution
{
    /*!
    *   @brief Distribute data to all the available receivers from the receiver pool.
    *   @param package Const data package that will be delived to all the receivers.
    *   @return true if the distribution succedded, false if any of the receiver fails to interpret the package.
    *   @note Data distribution will be performed sequencially.
    */
    virtual bool distributeData(DataPackageCPtr package) = 0;
};

//! Data distribution pointer
using DataDistributionPtr = std::shared_ptr<DataDistribution>;

struct DataDistributionStatistics
{
    virtual const uint64_t& getNumberOfProcessedPackagesPerSecond() = 0;
    virtual const uint64_t& getMaximumProcessedPackagesPerSecond() = 0;
};