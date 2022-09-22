#pragma once
#include <Log.hpp>
#include <MdsInterface.hpp>
#include <core/ConfigurationManager.hpp>
#include <core/DistributionManager.hpp>
#include <core/EngineObject.hpp>

namespace core
{
    /*!
    *   @brief metrics namespace
    */
    namespace metrics
    {
        class Watchdog;
    }
}
namespace core
{
    /*!
    *   @brief MDS engine definition. The engine is responsable to initialize objects and to 
    * facilitate interface access between them. It is also responsable for object handling.
    */
    class Engine :
        public InterfaceAccess,
        public EngineInit,
        public std::enable_shared_from_this<Engine>
    {
        std::shared_ptr<ConfigurationManager> configMgr_; //!< configuration manager pointer
        std::shared_ptr<DistributionManager> dataDistributionPtr_; //!< data distribution manager pointer
        std::shared_ptr<LoggingInterface> logger_; //!< logging interface
        metrics::Watchdog* watchdog_; //!< watchdog pointer
        std::shared_ptr<EngineObject> self_; //!< mds engine mirror as measurement object.
        
    public:
        /*!
        *   @brief MDS engine contructor.
        *   @param flag Flag indicating how the engine will start
        */
        Engine(EngineInitFlag flag = EngineInitFlag::normal);

        /*!
        *   @brief MDS engine destructor
        */
        virtual ~Engine();

        /*!
        *   @brief Method that retreive the desired interface as a void pointer.
        *   @param interfaceName Interface name as a string
        *   @return Return a void pointer to the requested interface
        *   @warning Check if the returned pointer is not nullptr.
        */
        virtual void* getInterface(const std::string& interfaceName);

        /*!
        *   @brief Method use to initialize the engine component.
        *   @note Engine need to be initialized in order to create the configuration manager and the
        * data distribution manager.
        *   @warning make sure that the engine is terminated before deleting it.
        */
        virtual void initialize();

        /*!
        *   @brief Method used to terminate the engine component.
        *   @note Need to be called before the destructor.
        */
        virtual void terminate();

        /*!
        *   @brief Method used to obtain the configuration manager.
        *   @todo Replace the method by using the get interface method to obtain an interface to the
        * configuration manager
        *   @return Return a configuration manager reference.
        */
        std::shared_ptr<ConfigurationManager>& getConfigurationManager();

        /*!
        *   @brief Method used to obtain the packages that failed or passed the distribution process.
        *   @param pass a reference in which the number of passed packages counter will be put.
        *   @param fail a reference in which the number of failed packages counter will be put.
        */
        void getDistributionStatistics(size_t& pass, size_t& fail);

        /*!
        *   @brief Method that retreive the watchdog status.
        *   @return Return true if the watchdog is active, false otherwise.
        */
        bool isWatchDogActive();

        /*!
        *   @brief Method that retreive the logging status.
        *   @return Return true if the logging is active, false otherwise.
        */
        bool isLoggerActive();

        /*!
        *   @brief Method that retreive the data aquisition status.
        *   @return Return true if the data aquisition is performed, false otherwise.
        */
        bool isPerformingDataAquisition();

        friend class EngineUtilis; //!< for didactic usage only
    };
}