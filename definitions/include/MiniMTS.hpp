#pragma once
#include <MiniObjectDefs.hpp>
#include <filesystem>
#include <string_view>


struct InterfaceAccess
{
    virtual void* getInterface(std::string_view interfaceName) = 0;
};

struct EngineInit
{
    virtual void initialize() = 0;
    virtual void terminate() = 0;
};

enum class EngineInitFlag : uint8_t
{
    normal = 0x00,
    silent = 0x01,
    no_metrics = 0x02,
    no_exception_handler = 0x04,
    performance = 0x07,
};

struct ConfigurationParser
{
    virtual const MeasurementObjectList& loadConfiguration(std::filesystem::path path) = 0;
};