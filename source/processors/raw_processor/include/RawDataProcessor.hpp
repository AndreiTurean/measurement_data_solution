#pragma once
#include <visibility.h>
#include <defs/MdsInterface.hpp>
#include <defs/MeasurementObjectDefs.hpp>
#include <defs/Receiver.hpp>
#include <set>

namespace processors
{
    class DUMMY_API RawDataProcessor :
        public MeasurementObject,
        public DataReceiverObject,
        public InterfaceAccess,
        public ReceiverSinkManager
    {
        InterfaceAccess* interfaceAccess_;
        uint8_t instanceNb_;
        uint64_t handle_;
        std::string name_;
        MeasurementObjectType type_;
        std::set<NotifySubjects*> subjects_;

    public:
        RawDataProcessor(InterfaceAccess* interfaceAccess, uint8_t nb, const std::string& name);
        virtual ~RawDataProcessor();

        //! MeasurementObject interface implementation
        virtual const uint8_t& getInstanceNumber();
        virtual const uint64_t& getHandle();
        virtual const MeasurementObjectType& getType();
        virtual const std::string& getName();

        //! DataReceiverObject interface implementation
        virtual bool validatePackage(DataPackageCPtr package);

        //! InterfaceAccess interface implementation
        virtual void* getInterface(const std::string& interfaceName);

        virtual bool registerToReceiverSink(NotifySubjects* subject);
        virtual bool unregisterToReceiverSink(NotifySubjects* subject);
        
    };
}

extern "C" DUMMY_API MeasurementObjectPtr createMO(InterfaceAccess* interfaceAccess, const uint8_t instanceNb, const char* name)
{
    return new processors::RawDataProcessor(interfaceAccess, instanceNb, name);
}