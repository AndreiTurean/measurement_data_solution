#pragma once
#include <MiniMTS.hpp>

namespace core
{
    class EngineObject:
        public MeasurementObject
    {
        uint8_t instanceNumber_;
        uint64_t handle_;
        std::string name_;
        MeasurementObjectType type_;
    public:
        EngineObject();
        virtual const uint8_t& getInstanceNumber();
        virtual const uint64_t& getHandle();
        virtual const MeasurementObjectType& getType();
        virtual const std::string& getName();

    };
}