#pragma once
#include <MiniMTS.hpp>
#include <core/ConfigurationManager.hpp>
#include <Log.hpp>
#include <core/DistributionManager.hpp>

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
        public std::enable_shared_from_this<Engine>,
        public MeasurementObject
    {
        std::shared_ptr<ConfigurationManager> configMgr_;
        std::shared_ptr<DistributionManager> dataDistributionPtr_;
        std::shared_ptr<LoggingInterface> logger_;
        metrics::Watchdog* watchdog_;
        uint8_t instanceNumber_;
        std::string name_;
        uint64_t handle_;
        MeasurementObjectType type_;
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

        virtual const uint8_t& getInstanceNumber();
        virtual const uint64_t& getHandle();
        virtual const MeasurementObjectType& getType();
        virtual const std::string& getName();
    };
}