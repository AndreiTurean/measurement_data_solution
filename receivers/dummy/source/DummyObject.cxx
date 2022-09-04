#include <DummyObject.hpp>
#include <iostream>

namespace receivers
{
        DummyObject::DummyObject(InterfaceAccess* interfaceAccess, uint8_t nb, uint64_t handle, const std::string& name):
            interfaceAccess_(interfaceAccess),
            instanceNb_(nb),
            handle_(handle),
            name_(name),
            type_(MeasurementObjectType::data_receiver)
        {

        }

        const uint8_t& DummyObject::getInstanceNumber()
        {
            return instanceNb_;
        }
        const uint64_t& DummyObject::getHandle()
        {
            return handle_;
        }
        const MeasurementObjectType& DummyObject::getType()
        {
            return type_;
        }
        const std::string& DummyObject::getName()
        {
             return name_;
        }

        bool DummyObject::validatePackage(DataPackageCPtr pkg)
        {
            std::cout<<"pkg timestamp:" << pkg->timestamp <<std::endl;
            return false;
        }

        void* DummyObject::getInterface(const std::string&)
        {
            return nullptr;
        }
}

std::shared_ptr<MeasurementObject> createMO(InterfaceAccess* interfaceAccess, const uint8_t instanceNb, uint64_t handle, const char* name)
{
	return std::make_shared<receivers::DummyObject>(interfaceAccess,instanceNb,handle,name);
}