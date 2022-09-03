#include <core/DistributionManager.hpp>

namespace core
{
    DistributionManager::DistributionManager(InterfaceAccess* ifcAccess)
        :   interfaceAccess_(ifcAccess)
    {

    }

    bool DistributionManager::distributeData(DataPackageCPtr package)
    {
        if(receiversPool_.empty())
        {
            return false;
        }
        bool distributionResult = true;
        for(const auto& receiver : receiversPool_)
        {
            distributionResult &= receiver->validatePackage(package);
        }

        return distributionResult;
    }
    void DistributionManager::addReceiver(DataReceiverObjectPtr object)
    {
        receiversPool_.push_back(object);
    }

    void* DistributionManager::getInterface(std::string_view interfaceName)
    {
        if(interfaceAccess_)
            return interfaceAccess_->getInterface(interfaceName);
        if(interfaceName == "DataDistribution")
            return dynamic_cast<DataDistribution*>(this);

        return nullptr;
    }
}