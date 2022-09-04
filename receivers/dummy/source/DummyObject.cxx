#include <DummyObject.hpp>

namespace receivers
{
        DummyObject::DummyObject(InterfaceAccess* interfaceAccess, uint8_t nb, uint64_t handle, const std::string& name):
                interfaceAccess_(interfaceAccess),
				instanceNb_(nb),
				handle_(handle),
				name_(name)
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

        bool DummyObject::validatePackage(DataPackageCPtr)
        {

            return false;
        }

        void* DummyObject::getInterface(const std::string&)
        {
            return nullptr;
        }
}

MeasurementObject* createMO(InterfaceAccess* interfaceAccess, const uint8_t instanceNb, uint64_t handle, const char* name)
{
	return new receivers::DummyObject(interfaceAccess,instanceNb,handle,name);
}