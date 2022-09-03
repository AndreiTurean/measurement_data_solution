#include <transmitters/DummyObject.hpp>

namespace transmitters
{
    Dummy::Dummy(InterfaceAccess* interfaceAccess, const uint8_t instanceNb, uint64_t handle, std::string_view name) :
        interfaceAccess_(interfaceAccess),
        instanceNumber_(instanceNb),
        handle_(handle),
        name_(name),
        type_(MeasurementObjectType::data_source)
    {

    }
    void* Dummy::getInterface(std::string_view)
    {
        return nullptr;
    }
    DataPackagePtr Dummy::sendPackage()
    {
        return std::make_shared<DataPackage>();
    }
    const uint8_t& Dummy::getInstanceNumber()
    {
        return instanceNumber_;
    }
    const uint64_t& Dummy::getHandle()
    {
        return handle_;
    }
    const MeasurementObjectType& Dummy::getType()
    {
        return type_;
    }
    std::string_view Dummy::getName()
    {
        return name_;
    }
}

std::shared_ptr<MeasurementObject> createMO(InterfaceAccess* interfaceAccess, const uint8_t instanceNb, uint64_t handle, std::string_view name)
{
	return std::make_shared<transmitters::Dummy>(interfaceAccess, instanceNb, handle, name);
}

const char* getName()
{
	return "DUMMY";
}