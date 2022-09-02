#pragma once

// STL headers
#include <string>

// Core headers
#include <MiniMTS.hpp>
#include <MiniObjectDefs.hpp>

// Replay headers
#include <PlayerDefs.hpp>

namespace replay
{
    class PlayerObject :
        public InterfaceAccess,
        public MeasurementObject
    {
    public:
        PlayerObject(InterfaceAccess* interfaceAccess);
        virtual ~PlayerObject();
        //! MeasurementObject interface implementation
        virtual const uint8_t& getInstanceNumber();
        virtual const uint64_t& getHandle();
        virtual const MeasurementObjectType& getType();
        virtual std::string_view getName();

        //! InterfaceAccess interface implementation
        virtual void* getInterface(std::string_view interfaceName);

    private:
        InterfaceAccess* interfaceAccess_;
        uint8_t instanceNumber_;
        uint64_t handle_;
        MeasurementObjectType measurementObjectType_;
        std::string name_;
    };
}

