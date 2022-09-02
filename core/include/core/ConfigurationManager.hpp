#pragma once
#include <MiniMTS.hpp>

namespace core
{
    class ConfigurationManager :
        public ConfigurationParser,
        public InterfaceAccess
    {
    public:
        virtual MeasurementObjectList loadConfiguration(std::filesystem::path path);
        virtual void* getInterface(std::string_view interfaceName);
    };
}