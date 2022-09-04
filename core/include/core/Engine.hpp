#pragma once
#include <MiniMTS.hpp>
#include <core/ConfigurationManager.hpp>

namespace core
{
    class Engine :
        public InterfaceAccess,
        public EngineInit
    {
        std::shared_ptr<ConfigurationManager> configMgr_;
        InterfaceAccess* dataDistributionPtr_;
    public:
        Engine(EngineInitFlag flag = EngineInitFlag::normal);
        ~Engine();
        Engine(const Engine& lhs);
        const Engine& operator=(const Engine& lhs);

        virtual void* getInterface(const std::string& interfaceName);
        virtual void initialize();
        virtual void terminate();
        std::shared_ptr<ConfigurationManager>& getConfigurationManager();
        void getDistributionStatistics(size_t& pass, size_t& fail);
    };
}