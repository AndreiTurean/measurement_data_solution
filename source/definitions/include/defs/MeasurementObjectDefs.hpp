#pragma once
#include <map>
#include <list>
#include <memory>
#include <cstdint>
#include <string>

/*!
*   @brief Measurement object type
*/
enum class MeasurementObjectType : uint8_t
{
    data_source, //!< object used for transmitting data to processors
    data_receiver, //!< object used for processing data and notifying all the subscribers
    player, //!< object used for reader a recording file and act as a transmitter
    recorder, //!< object used for recording data, can be present in visualizers for recording snapshots
    system, //!< system objectr. E.g. engine, configuration manager, factory ...
    visualization, //!< visualization objects, receive data from data_receiver
};
/*!
*   @brief Interface for retreiving measurement object information
*/
struct MeasurementObject
{
    /*!
    *   @brief Method to retreive the instance number of the measurement object.
    *   @return Return measurement object instance number.
    */
    virtual const uint8_t& getInstanceNumber() = 0;
    /*!
    *   @brief Method to retreive the handle of the measurement object.
    *   @return Return measurement object handle.
    */
    virtual const uint64_t& getHandle() = 0;
    /*!
    *   @brief Method to retreive the type of the measurement object.
    *   @return Return measurement object type.
    */
    virtual const MeasurementObjectType& getType() = 0;
    /*!
    *   @brief Method to retreive the name of the measurement object.
    *   @return Return measurement object name.
    */
    virtual const std::string& getName() = 0;
};

//! MO pointer
using MeasurementObjectPtr = MeasurementObject*;
//! List of measurement objects
using MeasurementObjectList = std::list<MeasurementObjectPtr>;

using PropertyTable = std::map<std::string, std::string>;
using PropertyPair= std::pair<std::string, std::string>;

struct ExtendedMeasurementObject : public MeasurementObject
{
    virtual bool hasPropertyTable() = 0;
    virtual bool insertEntry(const PropertyPair& entryPair) = 0;
    virtual bool removeProperty(const std::string& propertyName) = 0;
    virtual void clearPropertyTable() = 0;
    virtual const PropertyTable& getPropertyTable() = 0;
    virtual const std::string& getPropertyEntryValue(const std::string& entry) = 0;
};