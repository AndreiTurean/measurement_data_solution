#pragma once
#include <MiniMTS.hpp>
#include <Log.hpp>
#include <core/MeasurementObjectFactory.hpp>

namespace core
{
    class ConfigurationManager :
        public ConfigurationParser,
        public InterfaceAccess,
        public std::enable_shared_from_this<ConfigurationManager>
    {
        InterfaceAccess* interfaceAccess_;
        MeasurementObjectList measurementObjectList_;
        std::shared_ptr<MeasurementObjectFactory> factory_;
        LoggingInterface* logger_;
    public:
        ConfigurationManager(InterfaceAccess* interfaceAccess, std::shared_ptr<MeasurementObjectFactory> factory);
        virtual const MeasurementObjectList& loadConfiguration(std::filesystem::path path);
        virtual void* getInterface(const std::string& interfaceName);
        bool createMeasurementObject(const std::string& name, uint8_t instanceNb);
        bool createMeasurementObject(MeasurementObjectPtr object);
        bool removeMeasurementObject(const std::string& name);
        const MeasurementObjectList& getMOsAddedInConfig();
        size_t getFactorySize();
        void clearMeasurementObjectList();
        std::vector<std::string> getFactoryObjectList();

        friend class EngineUtilis;
    };
}