#pragma once
#include <map>
#include <string>
#include <filesystem>

#include <MdsInterface.hpp>
#include <utilis/LibUtility.hpp>

namespace core
{
    class MeasurementObjectFactory:
        public std::enable_shared_from_this<MeasurementObjectFactory>
    {
        std::map<std::string, void*> objectsMap_;
        core::utility::LibUtility utilityLibrary_;
        InterfaceAccess* interfaceAccess_;
        void scanForMeasurementObjects(std::filesystem::path path);
    public:
        explicit MeasurementObjectFactory(InterfaceAccess* interfaceAccess);
        
        std::shared_ptr<MeasurementObject> createMeasurementObject(const std::string& name, uint8_t instanceNb);
        size_t getExtractedFuncSize();
        std::vector<std::string> getFactoryList();

        friend class EngineUtilis;
    };
}