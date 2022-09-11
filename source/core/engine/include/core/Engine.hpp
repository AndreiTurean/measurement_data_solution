#pragma once
#include <MiniMTS.hpp>
#include <core/ConfigurationManager.hpp>
#include <Log.hpp>
#include <core/DistributionManager.hpp>
#include <core/EngineObject.hpp>

namespace core
{
    namespace metrics
    {
        class Watchdog;
    }
}
namespace core
{
    class Engine :
        public InterfaceAccess,
        public EngineInit,
        public std::enable_shared_from_this<Engine>
    {
        std::shared_ptr<ConfigurationManager> configMgr_;
        std::shared_ptr<DistributionManager> dataDistributionPtr_;
        std::shared_ptr<LoggingInterface> logger_;
        metrics::Watchdog* watchdog_;
        std::shared_ptr<EngineObject> self_;
        
    public:
        Engine(EngineInitFlag flag = EngineInitFlag::normal);
        virtual ~Engine();

        virtual void* getInterface(const std::string& interfaceName);
        virtual void initialize();
        virtual void terminate();
        std::shared_ptr<ConfigurationManager>& getConfigurationManager();
        void getDistributionStatistics(size_t& pass, size_t& fail);
        bool isWatchDogActive();
        bool isLoggerActive();
        bool isPerformingDataAquisition();

        friend class EngineUtilis;
    };
}