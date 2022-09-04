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
        virtual void* getInterface(const std::string& interfaceName);
        bool createMeasurementObject(const std::string& name, uint8_t instanceNb, uint64_t handle);
        bool removeMeasurementObject(const std::string& name);
        const MeasurementObjectList& getMOsAddedInConfig();
        size_t getFactorySize();
    };
}