#pragma once

#include <MiniMTS.hpp>

namespace core
{
    class Engine :
        public InterfaceAccess,
        public EngineInit
    {
    public:
        Engine(EngineInitFlag flag = EngineInitFlag::normal);
        virtual void* getInterface(std::string_view interfaceName);
        virtual void initialize();
        virtual void terminate();
    };
}