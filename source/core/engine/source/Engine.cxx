#include <core/Engine.hpp>
#include <core/MeasurementObjectFactory.hpp>
#include <core/DistributionManager.hpp>
#include <cstring>
#include <Logger.hpp>
#include "Watchdog.hpp"

namespace core
{
    Engine::Engine(EngineInitFlag flag):
        configMgr_(nullptr),
        dataDistributionPtr_(nullptr),
        logger_(nullptr),
        watchdog_(nullptr),
        self_(nullptr),
        interfaceHelperPtr_(nullptr)
    {
        bool silentLog = false;
        bool silenceWatchDog = false;
        switch (flag)
        {
        case EngineInitFlag::normal:
            break;
        case EngineInitFlag::silent:
        {
            silentLog = true;
        }
        break;
        case EngineInitFlag::no_metrics:
        {
            silenceWatchDog = true;
        }
        break;
        case EngineInitFlag::performance:
        {
            silentLog = true;
            silenceWatchDog = true;
        }
        break;
        default:
            break;
        }
        interfaceHelperPtr_ = new core::utility::InterfaceUtilityHelper(this);
        logger_ = new Logger(this, silentLog);
        dataDistributionPtr_ = new DistributionManager(this);

        if(!silenceWatchDog)
        {
            watchdog_ = new metrics::Watchdog(logger_);
        }
    }
    Engine::~Engine()
    {
        terminate();
    }


    void* Engine::getInterface(const std::string& ifcName)
    {
        if(ifcName.find("LoggingInterface") != std::string::npos)
        {
            return dynamic_cast<LoggingInterface*>(logger_);
        }
        if (ifcName.find("DataDistributionStatistics") != std::string::npos)
        {
            return dataDistributionPtr_->getDistributionInterface();
        }
        if (ifcName.find("DistributionManagerPrivate") != std::string::npos)
        {
            return dynamic_cast<DistributionManagerPrivate*>(dataDistributionPtr_);
        }
        if(ifcName.find("DataDistribution") != std::string::npos)
        {
            return dynamic_cast<DataDistribution*>(dataDistributionPtr_);
        }
        if(ifcName.find("ConfigurationParser") != std::string::npos)
        {
            return dynamic_cast<ConfigurationParser*>(configMgr_);
        }
        if(ifcName.find("ConfigurationFactory") != std::string::npos)
        {
            return configMgr_->getInterface(ifcName);
        }

        return nullptr;
    }

    void Engine::initialize()
    {
        logger_->log("Started initialization");
        std::shared_ptr<MeasurementObjectFactory> factory = std::make_shared<MeasurementObjectFactory>(this);
        configMgr_ = new ConfigurationManager(this, factory);
        self_ = new EngineObject();
        if(!configMgr_->createMeasurementObject(self_))
        {
            logger_->log("Failed to introduce Engine Object in the configuration manager", ENGINE_HANDLE, severity::critical);
        }
        logger_->log("Initialization finished");
    }
    
    void Engine::terminate()
    {
        logger_->log("Started engine termination");
        if(watchdog_)
        {
            logger_->log("Destroying the watchdog");
            //delete watchdog_;
        }
        
        if (dataDistributionPtr_)
        {
            dataDistributionPtr_->stopDistribution();
        }

        if (configMgr_)
        {
            configMgr_->terminate();
            delete configMgr_;
            configMgr_ = nullptr;
        }

        if (dataDistributionPtr_)
        {
            delete dataDistributionPtr_;
            dataDistributionPtr_ = nullptr;
        }
    }

    bool Engine::isWatchDogActive()
    {
        return watchdog_ != nullptr;
    }

    bool Engine::isLoggerActive()
    {
        return logger_ != nullptr;
    }
    bool Engine::isPerformingDataAquisition()
    {
        return dataDistributionPtr_->isDistributing();
    }
}