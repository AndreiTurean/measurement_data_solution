#include <core/Engine.hpp>
#include <core/MeasurementObjectFactory.hpp>

namespace core
{
    Engine::Engine(EngineInitFlag flag)
    {
        switch (flag)
        {
        default:
            break;
        }
    }

    void* Engine::getInterface(std::string_view)
    {
        return nullptr;
    }

    void Engine::initialize()
    {
        std::shared_ptr<MeasurementObjectFactory> factory = std::make_shared<MeasurementObjectFactory>();
        factory->scanForMeasurementObjects(std::filesystem::current_path());
        configMgr_ = std::make_shared<ConfigurationManager>(this, factory);
    }
    
    void Engine::terminate()
    {
        configMgr_.reset();
    }

    std::shared_ptr<ConfigurationManager>& Engine::getConfigurationManager()
    {
        return configMgr_;
    }
}