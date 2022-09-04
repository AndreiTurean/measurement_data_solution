#pragma once
#include <MiniObjectDefs.hpp>
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

using DataDistributionPtr = std::shared_ptr<DataDistribution>;