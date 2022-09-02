#pragma once
#include <MiniObjectDefs.hpp>

struct DataDistribution
{
    virtual bool distributeData(DataPackageCPtr package) = 0;
    virtual void addReceiver(DataReceiverObject object) = 0;
};