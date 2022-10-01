#pragma once
#include <defs/MiniObjectDefs.hpp>
#include <filesystem>
#include <string>

/*!
*   @brief Interface helper that facilitate getting other interfaces.
*/
struct InterfaceAccess
{
    /*!
    *   @brief Retreive a pointer to the desired interface.
    *   @param interfaceName Name of the interface required.
    *   @return a pointer to the requested interface
    *   @warning check the pointer before using it. If the interface doesn't exist it will return a nullptr.
    */
    virtual void* getInterface(const std::string& interfaceName) = 0;
};

/*!
*   @brief Interface used to initialize MDS Engine
*/
struct EngineInit
{
    /*!
    *   @brief Initialize the engine with all its components
    */
    virtual void initialize() = 0;
    /*!
    *   @brief Destroy the engine with all its dependencies created.
    */
    virtual void terminate() = 0;
};

/*!
*   @brief Flag indicating how the engine will be created.
*/
enum class EngineInitFlag : uint8_t
{
    normal = 0x00, //!< engine will start normaly with all its components.
    silent = 0x01, //!< engine will inform the logger to not log debug messages.
    no_metrics = 0x02, //!< engine won't create a watchdog.
    no_exception_handler = 0x04, //!< some safety casts will be ignored.
    performance = 0x07, //!< engine will run with the following flags: silent | no_metrics | no_exception_handler
};

/*!
*   @brief Interface used to load a configurarion file.
*/
struct ConfigurationParser
{
    /*!
    *   @brief loads all the objects with their properties into the configuration manager.
    *   @param path path to the configuration file. The full path must be provided.
    *   @return Return a const list of measurement objects.
    *   @note When loading a configuration, engine will be reseted and reinitialized.
    */
    virtual const MeasurementObjectList& loadConfiguration(std::filesystem::path path) = 0;
    virtual bool createMeasurementObject(const std::string& name, uint8_t instanceNb) = 0;
    /*!
    *   @brief Method used to introduce an already created measurement object into the configuration manager.
    *   @param object Already created measurement object that will be inserted into the configuration manager.
    *   @return Return true if the object was inserted correctly, false otherwise.
    */
    virtual bool createMeasurementObject(MeasurementObjectPtr object) = 0;

    /*!
    *   @brief Method used to remove a measurement object from the configuration manager.
    *   @param name Measurement object name
    *   @return True if the 
    */
    virtual bool removeMeasurementObject(const std::string& name) = 0;

    /*!
    *   @brief Retreive the active measurement object lists from the configuration manager.
    *   @return Return a const reference of the measurement object list.
    */
    virtual const MeasurementObjectList& getMOsAddedInConfig() = 0;

    /*!
    *   @brief Get the factory size. The factory size represents the number of unique measurement
    * objects that can be created. The factory is populated using dlopen methods.
    *   @return Return the factory size.
    *   @note For more information about the factory read MeasurementObjectFactory class definitions.
    */
    virtual size_t getFactorySize() = 0;
};

/***************************************
handle definitions
***************************************/
#define INVALID_HANDLE 0xffffffffffffffff
#define ENGINE_HANDLE 0x00
#define CONFIGURATION_MGR_HANDLE 0x01
#define DISTRIBUTION_MGR_HANDLE 0x02
#define WATCHDOG_HANDLE 0x03
#define FACTORY_HANDLE 0x04

