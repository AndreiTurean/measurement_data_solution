#pragma once
#include <MiniMTS.hpp>
#include <MiniObjectDefs.hpp>
#include <Distribution.hpp>

namespace core
{
    class DistributionManager :
        public InterfaceAccess,
        public DataDistribution
    {
        InterfaceAccess* interfaceAccess_;
        std::list<DataReceiverObjectPtr> receiversPool_;
    public:
        DistributionManager(InterfaceAccess* ifcAccess);

        //! DataDistribution interface implementation
        bool distributeData(DataPackageCPtr package);
        void addReceiver(DataReceiverObjectPtr object);

        //! InterfaceAccess interface implementation
        virtual void* getInterface(std::string_view interfaceName);

    };
}