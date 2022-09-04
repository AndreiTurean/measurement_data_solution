#pragma once
#include <MiniMTS.hpp>
#include <vector>
#include <string_view>
#include <filesystem>
#include <functional>
#include <map>
#include <string>
#include <visibility.h>

namespace core
{
    class MeasurementObjectFactory
    {
        std::map<std::string, void*> objectsMap_;
    public:
        MeasurementObjectFactory();
        void scanForMeasurementObjects(std::filesystem::path path);
        std::shared_ptr<MeasurementObject> createMeasurementObject(const std::string& name, uint8_t instanceNb, uint64_t handle);
        size_t getExtractedFuncSize();
    };
}