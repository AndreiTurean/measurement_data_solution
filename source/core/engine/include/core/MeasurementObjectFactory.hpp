#pragma once
#include <map>
#include <string>
#include <filesystem>

#include <defs/MdsInterface.hpp>
#include <defs/Log.hpp>
#include <utilis/LibUtility.hpp>

namespace core
{
    /*!
    *   @brief Measurement object factory definition
    */
    class MeasurementObjectFactory:
        public std::enable_shared_from_this<MeasurementObjectFactory>
    {
        std::map<std::string, void*> objectsMap_; //!< map containing factory defs and their shared library name
        core::utility::LibUtility utilityLibrary_; //!< utility library instance. Use to achive cross platform compilation
        InterfaceAccess* interfaceAccess_; //!< interface access pointer
        LoggingInterface* logger_;

        /*!
        *   @brief Method use to scan the received path for any shared libraries that can be opened.
        *   @param path Path where all the shared libraries will be loaded.
        */
        void scanForMeasurementObjects(std::filesystem::path path);
    public:
        /*!
        *   @brief Measurement object factory constructor
        *   @param interfaceAccess interface access pointer
        */
        explicit MeasurementObjectFactory(InterfaceAccess* interfaceAccess);
        
        /*!
        *   @brief Method used to use the factory method from the map to create measurement object based on the shared
        * library name.
        *   @param name Shared library name.
        *   @param instanceNb Measurement object instance number.
        *   @return Return a shared instance of the measurement object.
        */
        std::shared_ptr<MeasurementObject> createMeasurementObject(const std::string& name, uint8_t instanceNb);

        /*!
        *   @brief Method use to retreive the factory func size.
        *   @return Return factory func size.
        */
        size_t getExtractedFuncSize();

        /*!
        *   @brief Method used to obtain factory names.
        *   @return Return a vector containing the names of the shared libraries that can create a measurement object.
        */
        std::vector<std::string> getFactoryList();

        friend class EngineUtilis; //!< only for didactic purposes only.
    };
}