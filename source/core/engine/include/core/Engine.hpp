#pragma once
#include <defs/Log.hpp>
#include <defs/MdsInterface.hpp>
#include <core/EngineObject.hpp>
#include <core/DistributionManager.hpp>
#include <core/ConfigurationManager.hpp>
#include <utilis/InterfaceUtilityHelper.hpp>

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
        public EngineInit
    {
        ConfigurationManager* configMgr_; //!< configuration manager pointer
        DistributionManager* dataDistributionPtr_; //!< data distribution manager pointer
        LoggingInterface* logger_; //!< logging interface
        metrics::Watchdog* watchdog_; //!< watchdog pointerinterfaceHelperPtr_
        EngineObject* self_; //!< mds engine mirror as measurement object.
        core::utility::InterfaceUtilityHelper* interfaceHelperPtr_; //!< engine interface helper
        
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
        virtual void* getInterface(const std::string& interfaceName) override;

        /*!
        *   @brief Method use to initialize the engine component.
        *   @note Engine need to be initialized in order to create the configuration manager and the
        * data distribution manager.
        *   @warning make sure that the engine is terminated before deleting it.
        */
        virtual void initialize() override;

        /*!
        *   @brief Method used to terminate the engine component.
        *   @note Need to be called before the destructor.
        */
        virtual void terminate() override;

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

        template <typename ifc> ifc* getInterface()
        {
            return interfaceHelperPtr_->getInteface<ifc>();
        }

        core::utility::InterfaceUtilityHelper* getInterfaceHelper();

        friend class EngineUtilis; //!< for didactic usage only
    };
}