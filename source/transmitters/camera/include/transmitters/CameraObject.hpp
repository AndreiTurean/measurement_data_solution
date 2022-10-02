#pragma once
#include <visibility.h>
#include <defs/MdsInterface.hpp>
#include <defs/Distribution.hpp>
#include <memory>
#include <defs/Distribution.hpp>
#include <thread>
#include <mutex>

#include <transmitters/CameraProcessor.hpp>
#include <defs/Transmitter.hpp>

namespace transmitters
{
    class CAMERA_API CameraObject :
        public InterfaceAccess,
        public ExtendedMeasurementObject,
        public DataSenderObject
    {
        DataDistributionPtr dataDistributinonPtr_;
        InterfaceAccess* interfaceAccess_;
        uint8_t instanceNumber_;
        uint64_t handle_;
        std::string name_;
        MeasurementObjectType type_;
        DataDistribution* dataDistributionInterface_;
        PropertyTable propertyTable_;

    private:
        std::mutex processingMtx_;
        bool isProcessing_;
        std::unique_ptr<std::thread> processingThread_;
        std::unique_ptr<CameraProcessor> processor_;
        void doFSMProcessing();
        
    public:
        CameraObject(InterfaceAccess* interfaceAccess, const uint8_t instanceNb, const std::string& name);
        ~CameraObject();
        virtual void* getInterface(const std::string& interfaceName) override;
        virtual void startProcessing() override;
        virtual void endProcessing() override;
        virtual const uint8_t& getInstanceNumber() override;
        virtual const uint64_t& getHandle() override;
        virtual const MeasurementObjectType& getType() override;
        virtual const std::string& getName() override;
        virtual bool hasPropertyTable() override;
        virtual bool insertEntry(const PropertyPair& entryPair) override;
        virtual bool removeProperty(const std::string& propertyName) override;
        virtual void clearPropertyTable() override;
        virtual const PropertyTable& getPropertyTable() override;
        virtual const std::string& getPropertyEntryValue(const std::string& entry) override;
    };
}

extern "C"
{
    std::shared_ptr<MeasurementObject> CAMERA_API createMO(InterfaceAccess* interfaceAccess, const uint8_t instanceNb, const char* name)
    {
        return std::make_shared<transmitters::CameraObject>(interfaceAccess, instanceNb, name);
    }
}