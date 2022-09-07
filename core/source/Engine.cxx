#include <core/Engine.hpp>
#include <core/MeasurementObjectFactory.hpp>
#include <core/DistributionManager.hpp>
#include <cstring>

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
    }
    Engine::~Engine()
    {
        auto ifc = dynamic_cast<DistributionManager*>(this->dataDistributionPtr_);

        ifc->stopDistribution();
    }

    Engine::Engine(const Engine& lhs)
    {
        if(this->dataDistributionPtr_)
        {
            delete dynamic_cast<DistributionManager*>(this->dataDistributionPtr_);
        }

        this->dataDistributionPtr_ = new DistributionManager(this);

        std::memcpy(reinterpret_cast<void*>(this->dataDistributionPtr_),
            reinterpret_cast<void*>(lhs.dataDistributionPtr_), sizeof(DistributionManager));

        this->configMgr_ = lhs.configMgr_;
    }
    const Engine& Engine::operator=(const Engine& lhs)
    {
        if(this->dataDistributionPtr_)
        {
            delete dynamic_cast<DistributionManager*>(this->dataDistributionPtr_);
        }

        this->dataDistributionPtr_ = new DistributionManager(this);

        std::memcpy(reinterpret_cast<void*>(this->dataDistributionPtr_),
            reinterpret_cast<void*>(lhs.dataDistributionPtr_), sizeof(DistributionManager));

        this->configMgr_ = lhs.configMgr_;

        return *this;
    }

    void* Engine::getInterface(const std::string&)
    {
        return nullptr;
    }

    void Engine::initialize()
    {
        dataDistributionPtr_ = new DistributionManager(this);
        std::shared_ptr<MeasurementObjectFactory> factory = std::make_shared<MeasurementObjectFactory>(dataDistributionPtr_);
        factory->scanForMeasurementObjects(std::filesystem::current_path());
        configMgr_ = std::make_shared<ConfigurationManager>(dataDistributionPtr_, factory);
    }
    
    void Engine::terminate()
    {
        //configMgr_.reset();
    }

    std::shared_ptr<ConfigurationManager>& Engine::getConfigurationManager()
    {
        return configMgr_;
    }

    void Engine::getDistributionStatistics(size_t& pass, size_t& fail)
    {
        auto mgr = dynamic_cast<DistributionManager*>(this->dataDistributionPtr_);
        mgr->getDistributionStatistics(pass,fail);
    }
}