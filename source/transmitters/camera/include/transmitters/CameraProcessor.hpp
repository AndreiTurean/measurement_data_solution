#pragma once
#include <defs/DataPackage.hpp>
#include <transmitters/CameraAquisition.hpp>

namespace transmitters
{
    class CameraProcessor
    {
        CameraAquisition caAPI_;
    public:
        CameraProcessor();
        DataPackageCPtr getPackage();

    private:
        VideoHeader* getRawPackage();
    };
}