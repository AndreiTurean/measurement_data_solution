#pragma once
#include <list>
#include <memory>
#include <cstdint>
#include <string_view>

enum class MeasurementObjectType : uint8_t
{
    dummy,
    data_source,
    data_receiver,
    player,
    recorder,
};

struct MeasurementObject
{
    virtual const uint8_t& getInstanceNumber() = 0;
    virtual const uint64_t& getHandle() = 0;
    virtual const MeasurementObjectType& getType() = 0;
    virtual std::string_view getName() = 0;
};
using MeasurementObjectList = std::list<MeasurementObject>;

struct DataPackage
{
    uint64_t timestamp;
    uint64_t sourceHandle;
    size_t size;
    void* payload;
};
using DataPackagePtr = std::shared_ptr<DataPackage>;
using DataPackageCPtr = std::shared_ptr<const DataPackage>;

struct DataReceiverObject
{
    virtual bool validatePackage(DataPackageCPtr package) = 0;
};
using DataReceiverObjectPtr = std::shared_ptr<DataReceiverObject>;

struct DataSenderObject
{
    virtual void addPackage(DataPackagePtr package) = 0;
};
using DataSenderObjectPtr = std::shared_ptr<DataSenderObject>;
