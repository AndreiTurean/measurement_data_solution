#pragma once
#include <MiniMTS.hpp>
#include <MiniObjectDefs.hpp>
#include <Distribution.hpp>
#include <statistics/DistributionStatistics.hpp>
#include <mutex>
#include <Log.hpp>
#include <set>

namespace core
{
    struct DistributionManagerPrivate
    {
        virtual void addReceiver(DataReceiverObjectPtr object) = 0;
    };
    class DistributionManager :
        public InterfaceAccess,
        public DataDistribution,
        public DistributionManagerPrivate,
        public std::enable_shared_from_this<DistributionManager>
    {
        InterfaceAccess* interfaceAccess_;
        std::set<DataReceiverObjectPtr> receiversPool_;
        statistics::DistributionStatistics statistics_;
        std::mutex distributionLock_;
        bool distributing_;
        LoggingInterface* logger_;
    public:
        DistributionManager(InterfaceAccess* ifcAccess);
        virtual ~DistributionManager();


        //! DataDistribution interface implementation
        bool distributeData(DataPackageCPtr package);
        void addReceiver(DataReceiverObjectPtr object);

        //! InterfaceAccess interface implementation
        virtual void* getInterface(const std::string& interfaceName);
        void getDistributionStatistics(size_t& pass, size_t& fail);
        void stopDistribution();
        bool isDistributing();
    };
}