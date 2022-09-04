#pragma once
#include <visibility.h>
#include <MiniMTS.hpp>
#include <MiniObjectDefs.hpp>

namespace receivers
{
    class DummyObject :
        public MeasurementObject,
        public DataReceiverObject,
        public InterfaceAccess
    {
        InterfaceAccess* interfaceAccess_;
        uint8_t instanceNb_;
        uint64_t handle_;
        std::string name_;
        MeasurementObjectType type_;
    public:
        DummyObject(InterfaceAccess* interfaceAccess, uint8_t nb, uint64_t handle, const std::string& name);

        //! MeasurementObject interface implementation
        virtual const uint8_t& getInstanceNumber();
        virtual const uint64_t& getHandle();
        virtual const MeasurementObjectType& getType();
        virtual const std::string& getName();

        //! DataReceiverObject interface implementation
        virtual bool validatePackage(DataPackageCPtr package);

        //! InterfaceAccess interface implementation
        virtual void* getInterface(const std::string& interfaceName);
    };
}

extern "C" MeasurementObject* CORE_API createMO(InterfaceAccess* interfaceAccess, const uint8_t instanceNb, uint64_t handle, const char* name);