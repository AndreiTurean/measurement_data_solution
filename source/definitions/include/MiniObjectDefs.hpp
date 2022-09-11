#pragma once
#include <list>
#include <memory>
#include <cstdint>
#include <string>

enum class MeasurementObjectType : uint8_t
{
    data_source,
    data_receiver,
    player,
    recorder,
    system,
};

struct MeasurementObject
{
    virtual const uint8_t& getInstanceNumber() = 0;
    virtual const uint64_t& getHandle() = 0;
    virtual const MeasurementObjectType& getType() = 0;
    virtual const std::string& getName() = 0;
};
using MeasurementObjectPtr = std::shared_ptr<MeasurementObject>;
using MeasurementObjectList = std::list<std::shared_ptr<MeasurementObject>>;

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
    virtual void startProcessing() = 0;
    virtual void endProcessing() = 0;
};
using DataSenderObjectPtr = std::shared_ptr<DataSenderObject>;
