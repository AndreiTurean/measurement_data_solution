#pragma once
#include <MiniMTS.hpp>
#include <vector>
#include <string_view>
#include <filesystem>
#include <functional>
#include <map>
#include <string>
#include <visibility.h>
#include <utilis/LibUtility.hpp>

namespace core
{
    class MeasurementObjectFactory:
        public std::enable_shared_from_this<MeasurementObjectFactory>
    {
        std::map<std::string, void*> objectsMap_;
        core::utility::LibUtility utilityLibrary_;
        InterfaceAccess* interfaceAccess_;
    public:
        MeasurementObjectFactory(InterfaceAccess* interfaceAccess);
        void scanForMeasurementObjects(std::filesystem::path path);
        std::shared_ptr<MeasurementObject> createMeasurementObject(const std::string& name, uint8_t instanceNb, uint64_t handle);
        size_t getExtractedFuncSize();
    };
}