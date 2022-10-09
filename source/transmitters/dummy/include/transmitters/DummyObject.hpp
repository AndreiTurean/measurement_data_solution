#pragma once
#include <visibility.h>
#include <defs/MdsInterface.hpp>
#include <defs/Distribution.hpp>
#include <memory>
#include <defs/Distribution.hpp>
#include <defs/Transmitter.hpp>
#include <thread>
#include <mutex>

namespace transmitters
{
    class DUMMY_API Dummy :
        public InterfaceAccess,
        public MeasurementObject,
        public DataSenderObject
    {
        DataDistributionPtr dataDistributinonPtr_;
        InterfaceAccess* interfaceAccess_;
        uint8_t instanceNumber_;
        uint64_t handle_;
        std::string name_;
        MeasurementObjectType type_;
        DataDistribution* dataDistributionInterface_;

    private:
        std::mutex processingMtx_;
        bool isProcessing_;
        std::unique_ptr<std::thread> processingThread_;
        void doFSMProcessing();
        
    public:
        Dummy(InterfaceAccess* interfaceAccess, const uint8_t instanceNb, const std::string& name);
        virtual ~Dummy();
        virtual void* getInterface(const std::string& interfaceName);
        virtual void startProcessing();
        virtual void endProcessing();
        virtual const uint8_t& getInstanceNumber();
        virtual const uint64_t& getHandle();
        virtual const MeasurementObjectType& getType();
        virtual const std::string& getName();
    };
}

extern "C" DUMMY_API MeasurementObjectPtr createMO(InterfaceAccess* interfaceAccess, const uint8_t instanceNb, const char* name)
{
    return new transmitters::Dummy(interfaceAccess, instanceNb, name);
}
