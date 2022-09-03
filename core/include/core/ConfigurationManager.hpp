#pragma once
#include <MiniMTS.hpp>
#include <core/MeasurementObjectFactory.hpp>

namespace core
{
    class ConfigurationManager :
        public ConfigurationParser,
        public InterfaceAccess
    {
        InterfaceAccess* interfaceAccess_;
        MeasurementObjectList measurementObjectList_;
        std::shared_ptr<MeasurementObjectFactory> factory_;
    public:
        ConfigurationManager(InterfaceAccess* interfaceAccess, std::shared_ptr<MeasurementObjectFactory> factory);
        virtual const MeasurementObjectList& loadConfiguration(std::filesystem::path path);
        virtual void* getInterface(std::string_view interfaceName);
        bool createMeasurementObject(std::string_view name, uint8_t instanceNb, uint64_t handle, MeasurementObjectType type);
        bool removeMeasurementObject(std::string_view name);
        const MeasurementObjectList& getMOsAddedInConfig();
    };
}