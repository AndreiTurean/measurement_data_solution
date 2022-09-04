#include <core/DistributionManager.hpp>
#include <iostream>

namespace core
{
    DistributionManager::DistributionManager(InterfaceAccess* ifcAccess)
        :   interfaceAccess_(ifcAccess)
    {

    }
    DistributionManager::~DistributionManager()
    {
        interfaceAccess_ = nullptr;
    }

    DistributionManager::DistributionManager(const DistributionManager& lhs)
    {
        this->interfaceAccess_ = lhs.interfaceAccess_;
    }

    const DistributionManager& DistributionManager::operator=(const DistributionManager& lhs)
    {
        this->interfaceAccess_ = lhs.interfaceAccess_;
        return *this;
    }

    bool DistributionManager::distributeData(DataPackageCPtr package)
    {
        if(receiversPool_.empty())
        {
            statistics_.update(false);
            return false;
        }
        bool distributionResult = true;
        for(const auto& receiver : receiversPool_)
        {
            distributionResult &= receiver->validatePackage(package);
        }
        statistics_.update(distributionResult);
        return distributionResult;
    }
    void DistributionManager::addReceiver(DataReceiverObjectPtr object)
    {
        receiversPool_.push_back(object);
    }

    void* DistributionManager::getInterface(const std::string& interfaceName)
    {
        if(interfaceName == "DataDistribution")
            return dynamic_cast<DataDistribution*>(this);
        if(interfaceName == "DistributionManagerPrivate")
            return dynamic_cast<DistributionManagerPrivate*>(this);
        if(interfaceAccess_)
            return interfaceAccess_->getInterface(interfaceName);
        return nullptr;
    }

    void DistributionManager::getDistributionStatistics(size_t& pass, size_t& fail)
    {
        statistics_.reset(pass,fail);
    }
}