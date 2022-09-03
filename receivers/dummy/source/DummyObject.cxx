#include <DummyObject.hpp>

namespace receivers
{
        DummyObject::DummyObject(InterfaceAccess* interfaceAccess, uint8_t nb, uint64_t handle, std::string_view name):
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
        std::string_view DummyObject::getName()
        {
             return name_;
        }

        bool DummyObject::validatePackage(DataPackageCPtr)
        {

            return false;
        }

        void* DummyObject::getInterface(std::string_view)
        {
            return nullptr;
        }
}

MeasurementObject* createMO(InterfaceAccess* interfaceAccess, const uint8_t instanceNb, uint64_t handle, std::string_view name)
{
	return new receivers::DummyObject(interfaceAccess,instanceNb,handle,name);
}