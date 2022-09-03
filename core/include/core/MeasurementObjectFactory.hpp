#pragma once
#include <MiniMTS.hpp>
#include <vector>
#include <string_view>
#include <filesystem>
#include <functional>
#include <map>
#include <visibility.h>

namespace core
{
    class MeasurementObjectFactory
    {
        std::vector<std::string_view> binaryList_;
        std::map<std::string_view, void*> objectsMap_;
    public:
        MeasurementObjectFactory();
        void scanForMeasurementObjects(std::filesystem::path path);
        const std::vector<std::string_view>& getMeasurementObjectList();
        std::shared_ptr<MeasurementObject> createMeasurementObject(std::string_view name, uint8_t instanceNb, uint64_t handle, MeasurementObjectType type);
    };
}