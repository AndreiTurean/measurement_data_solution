#include <transmitters/CameraProcessor.hpp>

namespace transmitters
{
    DataPackageCPtr CameraProcessor::getPackageFromQueue()
    {
        auto tempPkg = queue_.top();
        queue_.pop();
        return tempPkg;
    }
}