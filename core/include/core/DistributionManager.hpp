#pragma once
#include <MiniMTS.hpp>
#include <MiniObjectDefs.hpp>
#include <Distribution.hpp>
#include <statistics/DistributionStatistics.hpp>
namespace core
{
    struct DistributionManagerPrivate
    {
        virtual void addReceiver(DataReceiverObjectPtr object) = 0;
    };
    class DistributionManager :
        public InterfaceAccess,
        public DataDistribution,
        public DistributionManagerPrivate
    {
        InterfaceAccess* interfaceAccess_;
        std::list<DataReceiverObjectPtr> receiversPool_;
        statistics::DistributionStatistics statistics_;
    public:
        DistributionManager(InterfaceAccess* ifcAccess);
        virtual ~DistributionManager();
        DistributionManager(const DistributionManager& lhs);
        const DistributionManager& operator=(const DistributionManager& lhs);


        //! DataDistribution interface implementation
        bool distributeData(DataPackageCPtr package);
        void addReceiver(DataReceiverObjectPtr object);

        //! InterfaceAccess interface implementation
        virtual void* getInterface(const std::string& interfaceName);
        void getDistributionStatistics(size_t& pass, size_t& fail);

    };
}