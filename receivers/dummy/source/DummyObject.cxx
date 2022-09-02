#include <DummyObject.hpp>

namespace receivers
{
        DummyObject::DummyObject(InterfaceAccess* interfaceAccess)
        {

        }

        //! MeasurementObject interface implementation
        const uint8_t& DummyObject::getInstanceNumber()
        {

        }
        const uint64_t& DummyObject::getHandle()
        {

        }
        const MeasurementObjectType& DummyObject::getType()
        {

        }
        std::string_view getName()
        {

        }

        //! DataReceiverObject interface implementation
        bool validatePackage(DataPackageCPtr package)
        {

        }

        //! InterfaceAccess interface implementation
        void* getInterface(std::string_view interfaceName)
        {
            
        }
}