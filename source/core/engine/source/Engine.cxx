#include <core/Engine.hpp>
#include <core/MeasurementObjectFactory.hpp>
#include <core/DistributionManager.hpp>
#include <cstring>
#include <Logger.hpp>
#include "Watchdog.hpp"

namespace core
{
    Engine::Engine(EngineInitFlag flag):
        dataDistributionPtr_(nullptr)
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
        logger_ = std::make_shared<Logger>(this, silentLog);
        dataDistributionPtr_ = std::make_shared<DistributionManager>(this);

        if(!silenceWatchDog)
        {
            watchdog_ = new metrics::Watchdog(logger_);
        }
    }
    Engine::~Engine()
    {
    }


    void* Engine::getInterface(const std::string& ifcName)
    {
        if(ifcName == "LoggingInterface")
        {
            return std::dynamic_pointer_cast<LoggingInterface>(logger_).get();
        }
        if(ifcName == "DataDistribution")
        {
            return std::dynamic_pointer_cast<DataDistribution>(dataDistributionPtr_).get();
        }
        if(ifcName == "DistributionManagerPrivate")
        {
            return std::dynamic_pointer_cast<DistributionManagerPrivate>(dataDistributionPtr_).get();
        }

        return nullptr;
    }

    void Engine::initialize()
    {
        logger_->log("Started initialization");
        std::shared_ptr<MeasurementObjectFactory> factory = std::make_shared<MeasurementObjectFactory>(this);
        configMgr_ = std::make_shared<ConfigurationManager>(this, factory);
        self_ = std::make_shared<EngineObject>();
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
        
        dataDistributionPtr_->stopDistribution();
        dataDistributionPtr_.reset();
        configMgr_->terminate();
        configMgr_.reset();
        self_.reset();
        logger_.reset();
    }

    std::shared_ptr<ConfigurationManager>& Engine::getConfigurationManager()
    {
        return configMgr_;
    }

    void Engine::getDistributionStatistics(size_t& pass, size_t& fail)
    {
        dataDistributionPtr_->getDistributionStatistics(pass,fail);
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