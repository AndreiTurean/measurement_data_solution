#pragma once
#include <defs/MiniObjectDefs.hpp>
#include <queue>
#include <functional>
namespace transmitters
{
    class CameraProcessor
    {
        std::priority_queue<DataPackagePtr> queue_;
    public:

        DataPackageCPtr getPackageFromQueue();
    };
}