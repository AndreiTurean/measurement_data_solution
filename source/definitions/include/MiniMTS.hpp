#pragma once
#include <MiniObjectDefs.hpp>
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
};

#define INVALID_HANDLE 0xffffffffffffffff
#define ENGINE_HANDLE 0x00
#define CONFIGURATION_MGR_HANDLE 0x01