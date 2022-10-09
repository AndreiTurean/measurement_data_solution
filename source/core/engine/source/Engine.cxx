#include <core/Engine.hpp>
#include <core/MeasurementObjectFactory.hpp>
#include <core/DistributionManager.hpp>
#include <cstring>
#include <Logger.hpp>
#include "Watchdog.hpp"

namespace core
{
    Engine::Engine(EngineInitFlag flag):
        dataDistributionPtr_(nullptr),
        self_(nullptr),
        watchdog_(nullptr),
        configMgr_(nullptr)
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
        if(ifcName == "LoggingInterface")
        {
            return dynamic_cast<LoggingInterface*>(logger_);
        }
        if(ifcName == "DataDistribution")
        {
            return dynamic_cast<DataDistribution*>(dataDistributionPtr_);
        }
        if(ifcName == "DistributionManagerPrivate")
        {
            return dynamic_cast<DistributionManagerPrivate*>(dataDistributionPtr_);
        }
        if(ifcName == "ConfigurationParser")
        {
            return dynamic_cast<ConfigurationParser*>(configMgr_);
        }
        if(ifcName == "ConfigurationFactory")
        {
            return configMgr_->getInterface(ifcName);
        }
        if(ifcName == "DataDistributionStatistics")
        {
            return dataDistributionPtr_->getDistributionInterface();
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
            delete watchdog_;
        }
        
        if (dataDistributionPtr_)
        {
            dataDistributionPtr_->stopDistribution();
            delete dataDistributionPtr_;
            dataDistributionPtr_ = nullptr;
        }

        if (configMgr_)
        {
            configMgr_->terminate();
            delete configMgr_;
            configMgr_ = nullptr;
        }
        if (self_)
        {
            delete self_;
            self_ = nullptr;
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