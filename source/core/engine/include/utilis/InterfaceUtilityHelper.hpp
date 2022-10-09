#pragma once
#include <defs/MdsInterface.hpp>

namespace core
{
    namespace utility
    {
        class InterfaceUtilityHelper
        {
        private:
            InterfaceAccess* interfaceAccessPtr_;
        public:
            InterfaceUtilityHelper(InterfaceAccess* ifc):
                interfaceAccessPtr_(ifc)
            {
            }

            template <typename ifc> ifc* getInteface()
            {
                return static_cast<ifc*>(interfaceAccessPtr_->getInterface(typeid(ifc).name()));
            }
        };
    }
}