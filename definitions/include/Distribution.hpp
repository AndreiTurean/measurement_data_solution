#pragma once
#include <MiniObjectDefs.hpp>

struct DataDistribution
{
    virtual bool distributeData(DataPackageCPtr package) = 0;
};

using DataDistributionPtr = std::shared_ptr<DataDistribution>;