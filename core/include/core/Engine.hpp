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
    public:
        Engine(EngineInitFlag flag = EngineInitFlag::normal);
        virtual void* getInterface(std::string_view interfaceName);
        virtual void initialize();
        virtual void terminate();
        std::shared_ptr<ConfigurationManager>& getConfigurationManager();
    };
}