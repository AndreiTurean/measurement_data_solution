#include <core/Engine.hpp>
#include <core/MeasurementObjectFactory.hpp>
#include <core/DistributionManager.hpp>
#include <cstring>
#include <Logger.hpp>

namespace core
{
    Engine::Engine(EngineInitFlag flag):
        dataDistributionPtr_(nullptr)
    {
        switch (flag)
        {
        default:
            break;
        }
        logger_ = std::make_shared<Logger>(this);
        dataDistributionPtr_ = std::make_shared<DistributionManager>(this);
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
        factory->scanForMeasurementObjects(std::filesystem::current_path());
        configMgr_ = std::make_shared<ConfigurationManager>(this, factory);
        logger_->log("Initialization finished");
    }
    
    void Engine::terminate()
    {
        logger_->log("Started engine termination");
        logger_.reset();
        dataDistributionPtr_->stopDistribution();
        dataDistributionPtr_.reset();
        configMgr_.reset();
    }

    std::shared_ptr<ConfigurationManager>& Engine::getConfigurationManager()
    {
        return configMgr_;
    }

    void Engine::getDistributionStatistics(size_t& pass, size_t& fail)
    {
        dataDistributionPtr_->getDistributionStatistics(pass,fail);
    }
}