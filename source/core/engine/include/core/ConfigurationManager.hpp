#pragma once
#include <defs/Log.hpp>
#include <defs/MdsInterface.hpp>
#include <defs/Configuration.hpp>
#include <core/MeasurementObjectFactory.hpp>

/*!
*   @brief Core namespace, used for core engine components.
*/
namespace core
{
    /*!
    *   @brief Configuration manager class. Responsible for managing all the measurement object.
    */
    class ConfigurationManager :
        public ConfigurationParser,
        public InterfaceAccess,
        public std::enable_shared_from_this<ConfigurationManager>
    {
        InterfaceAccess* interfaceAccess_; //!< interface access pointer
        MeasurementObjectList measurementObjectList_; //!< list containing all the measurement objects.
        std::shared_ptr<MeasurementObjectFactory> factory_; //!< pointer to the MeasurementObjectFactory, responsable for generating measurement objects.
        LoggingInterface* logger_; //!< logging interface
    public:
        /*!
        *   @brief Configuration manager contructor definition.
        *   @param interfaceAccess pointer to the interface access. Retreive any interface implmeneted by the caller (assuming caller is "this").
        *   @param factory  shared instance of the measurement object factory that is responsible for creating the measurement objects.
        */
        ConfigurationManager(InterfaceAccess* interfaceAccess, std::shared_ptr<MeasurementObjectFactory> factory);

        /*!
        *   @brief Loads a predefined configuration. Information for the engine will be taken from here and all the system components will be reseted.
        * The objects from the current configuration manager will be replaced with new ones from the parsed configuration file.
        *   @param path Path to the configuration file. The path must be complete and inclued the file.
        *   @return Return an constant reference to the new measurement object list. Used mostly for debug purpose and for resseting the engine and
        * the system components.
        *   @todo implement file reading and decide of the file format and structure.
        */
        virtual const MeasurementObjectList& loadConfiguration(std::filesystem::path path);

        /*!
        *   @brief Method that retreive the desired interface as a void pointer.
        *   @param interfaceName Interface name as a string
        *   @return Return a void pointer to the requested interface
        *   @warning Check if the returned pointer is not nullptr.
        */
        virtual void* getInterface(const std::string& interfaceName) override;
        /*!
        *   @brief Method that call the Measurement object factory and create a object based on name and instance number.
        *   @param name Measurement object name
        *   @param instanceNb Measurement object instance number
        *   @return Return true if the object was created succesfully, false otherwise.
        */
        virtual bool createMeasurementObject(const std::string& name, uint8_t instanceNb) override;
        /*!
        *   @brief Method used to introduce an already created measurement object into the configuration manager.
        *   @param object Already created measurement object that will be inserted into the configuration manager.
        *   @return Return true if the object was inserted correctly, false otherwise.
        */
        virtual bool createMeasurementObject(MeasurementObjectPtr object) override;

        /*!
        *   @brief Method used to remove a measurement object from the configuration manager.
        *   @param name Measurement object name
        *   @return True if the 
        */
        virtual bool removeMeasurementObject(const std::string& name) override;

        /*!
        *   @brief Retreive the active measurement object lists from the configuration manager.
        *   @return Return a const reference of the measurement object list.
        */
        virtual const MeasurementObjectList& getMOsAddedInConfig() override;

        /*!
        *   @brief Get the factory size. The factory size represents the number of unique measurement
        * objects that can be created. The factory is populated using dlopen methods.
        *   @return Return the factory size.
        *   @note For more information about the factory read MeasurementObjectFactory class definitions.
        */
        virtual size_t getFactorySize() override;

        /*!
        *   @brief Clear the measurement object list. This can be used when the configuration manager was loaded or
        * a new configuration file was loaded and the old measurement objects need to be destroyed.
        */
        void clearMeasurementObjectList();

        /*!
        *   @brief Return the name of all the loaded measurement object list.
        *   @todo To be deleted and replaced with a return of the factory map reference since there
        * is no need to populate a vector and to call twice the copy contructor. This cannot scale with
        * a big vector that contain big data strings.
        *   @return a copy of the measurement object that passed the dlopen.
        */
        std::vector<std::string> getFactoryObjectList();
        void terminate();
        friend class EngineUtilis; //!< Frined class used for creating a module test
    };
}