#include <core/EngineObject.hpp>

namespace core
{
    EngineObject::EngineObject():
        instanceNumber_(0),
        handle_(ENGINE_HANDLE),
        name_("Engine"),
        type_(MeasurementObjectType::system)
    {
    }

    const uint8_t& EngineObject::getInstanceNumber()
    {
        return instanceNumber_;
    }
    const uint64_t& EngineObject::getHandle()
    {
        return handle_;
    }
    const MeasurementObjectType& EngineObject::getType()
    {
        return type_;
    }
    const std::string& EngineObject::getName()
    {
        return name_;
    }
}