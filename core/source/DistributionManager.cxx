#include <core/DistributionManager.hpp>
#include <iostream>

namespace core
{
    DistributionManager::DistributionManager(InterfaceAccess* ifcAccess)
        :   interfaceAccess_(ifcAccess)
        ,   distributing_(true)
    {

    }
    DistributionManager::~DistributionManager()
    {
        std::lock_guard<std::mutex> lock(distributionLock_);
        receiversPool_.clear();
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
        std::lock_guard<std::mutex> lock(distributionLock_);

        if(!distributing_)
        {
            return false;
        }

        if(receiversPool_.empty())
        {
            return false;
        }
        bool distributionResult = true;
        for(auto receiver : receiversPool_)
        {
            distributionResult &= receiver->validatePackage(package);
        }
        statistics_.update(distributionResult);
        return distributionResult;
    }
    void DistributionManager::addReceiver(DataReceiverObjectPtr object)
    {
        std::lock_guard<std::mutex> lock(distributionLock_);
        receiversPool_.push_back(object);
    }

    void* DistributionManager::getInterface(const std::string& interfaceName)
    {
        std::lock_guard<std::mutex> lock(distributionLock_);
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
        std::lock_guard<std::mutex> lock(distributionLock_);
        statistics_.reset(pass,fail);
    }

    void DistributionManager::stopDistribution()
    {
        std::lock_guard<std::mutex> lock(distributionLock_);
        distributing_ = false;
    }
}