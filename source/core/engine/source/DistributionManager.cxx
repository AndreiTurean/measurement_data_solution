#include <core/DistributionManager.hpp>
#include <iostream>

namespace core
{
    DistributionManager::DistributionManager(InterfaceAccess* ifcAccess)
        : interfaceAccess_(ifcAccess)
        , distributing_(true)
        , statistics_(nullptr)
    {
        logger_ = static_cast<LoggingInterface*>(interfaceAccess_->getInterface("LoggingInterface"));
        logger_->subscribe("DistributionManager", DISTRIBUTION_MGR_HANDLE);

        statistics_ = new statistics::DistributionStatistics();
    }
    DistributionManager::~DistributionManager()
    {
        logger_->log("Destroying distribution manager", DISTRIBUTION_MGR_HANDLE);
        {
            std::lock_guard<std::mutex> lock(distributionLock_);
            distributing_ = false;
            receiversPool_.clear();
        }

        if(statistics_)
        {
            logger_->log("Destroying distribution statistics", DISTRIBUTION_MGR_HANDLE);
            delete statistics_;
        }
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
        statistics_->update(distributionResult);
        return distributionResult;
    }
    void DistributionManager::addReceiver(DataReceiverObjectPtr object)
    {
        std::lock_guard<std::mutex> lock(distributionLock_);
        receiversPool_.insert(object);
    }

    void* DistributionManager::getInterface(const std::string& interfaceName)
    {
        std::lock_guard<std::mutex> lock(distributionLock_);
        if(interfaceName == "DataDistribution")
            return dynamic_cast<DataDistribution*>(this);
        if(interfaceName == "DistributionManagerPrivate")
            return dynamic_cast<DistributionManagerPrivate*>(this);
        if(interfaceName == "DistributionManagerPrivate")
            return dynamic_cast<DistributionManagerPrivate*>(this);
        if(interfaceAccess_)
            return interfaceAccess_->getInterface(interfaceName);
        return nullptr;
    }

    void DistributionManager::stopDistribution()
    {
        std::lock_guard<std::mutex> lock(distributionLock_);
        logger_->log("Distribution of data was stopped", DISTRIBUTION_MGR_HANDLE);
        distributing_ = false;
        receiversPool_.clear();
    }

    bool DistributionManager::isDistributing()
    {
        std::lock_guard<std::mutex> lock(distributionLock_);
        return distributing_;
    }

    DataDistributionStatistics* DistributionManager::getDistributionInterface()
    {
        return statistics_;
    }
}