#pragma once

#include <MiniMTS.hpp>
#include <MiniObjectDefs.hpp>

namespace receivers
{
    class DummyObject :
        public MeasurementObject,
        public DataReceiverObject,
        public InterfaceAccess
    {
    public:
        DummyObject(InterfaceAccess* interfaceAccess);

        //! MeasurementObject interface implementation
        virtual const uint8_t& getInstanceNumber();
        virtual const uint64_t& getHandle();
        virtual const MeasurementObjectType& getType();
        virtual std::string_view getName();

        //! DataReceiverObject interface implementation
        virtual bool validatePackage(DataPackageCPtr package);

        //! InterfaceAccess interface implementation
        virtual void* getInterface(std::string_view interfaceName);
    };
}