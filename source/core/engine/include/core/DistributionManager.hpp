#pragma once
#include <set>
#include <mutex>

#include <Log.hpp>
#include <MdsInterface.hpp>
#include <MiniObjectDefs.hpp>
#include <Distribution.hpp>
#include <statistics/DistributionStatistics.hpp>

/*!
*   @brief Core namespace, used for core engine components.
*/
namespace core
{
    /*!
    *   @brief Interface used for adding data receiver into the distribution manager list.
    */
    struct DistributionManagerPrivate
    {
        /*!
        *   @brief Add a measurement object that is capable of receiving data packages into the
        * pool of receivers.
        *   @param object Measurement object that is capable of receiveing data packages.
        */
        virtual void addReceiver(DataReceiverObjectPtr object) = 0;
    };

    /*!
    *   @brief Data distribution manager class definition.
    */
    class DistributionManager :
        public InterfaceAccess,
        public DataDistribution,
        public DistributionManagerPrivate,
        public std::enable_shared_from_this<DistributionManager>
    {
        InterfaceAccess* interfaceAccess_; //!< interface access to parent interfaces
        std::set<DataReceiverObjectPtr> receiversPool_; //!< receivers pool that contains measuremet ojbect that can receive and process data
        statistics::DistributionStatistics statistics_; //!< data distribution statistics.
        std::mutex distributionLock_; //!< receivers pool lock. Used also for distribution flag.
        bool distributing_; //!< data distribution flag
        LoggingInterface* logger_; //!< logging interface
    public:
        /*!
        *   @brief Data distribution manager class constructor.
        *   @param ifcAccess Interface access pointer. Used to reach parent interfaces.
        */
        explicit DistributionManager(InterfaceAccess* ifcAccess);

        /*!
        *   @brief Data distribution manager class destructor.
        *   @note If the destruction of data manager is blocked then some thread are not finished
        * distributing data or cannot be joined.
        *   @todo Find a better solution for force stopping the data distribution in case the threads cannot
        * be joined.
        */
        virtual ~DistributionManager();

        /*!
        *   @brief Distribute data to all the available receivers from the receiver pool.
        *   @param package Const data package that will be delived to all the receivers.
        *   @return true if the distribution succedded, false if any of the receiver fails to interpret the package.
        *   @note Data distribution will be performed sequencially.
        */
        virtual bool distributeData(DataPackageCPtr package) override;

        /*!
        *   @brief Add a measurement object that is capable of receiving data packages into the
        * pool of receivers.
        *   @param object Measurement object that is capable of receiveing data packages.
        */
        virtual void addReceiver(DataReceiverObjectPtr object) override;

        /*!
        *   @brief Method that retreive the desired interface as a void pointer.
        *   @param interfaceName Interface name as a string
        *   @return Return a void pointer to the requested interface
        *   @warning Check if the returned pointer is not nullptr.
        */
        virtual void* getInterface(const std::string& interfaceName) override;

        /*!
        *   @brief Gets the distribution statistics regarding the passed/failed packages cnt.
        *   @todo replace this method with one data return a structure that contains real statistics
        * not just the pass / failed packages
        */
        void getDistributionStatistics(size_t& pass, size_t& fail);

        /*!
        *   @brief Stop the data distribution. All the data sources that continue to send packages
        * will not be added into the distribution manager and not passed to any receiver.
        */
        void stopDistribution();

        /*!
        *   @brief Method that retreive if the data distribution is going.
        *   @return Return true if the manager still accepts data from the data sources and pass it to
        * the receivers pool.
        */
        bool isDistributing();
    };
}