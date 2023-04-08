#pragma once
#include <map>
#include <list>
#include <memory>
#include <cstdint>
#include <string>

#define interface struct

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
interface MeasurementObject
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

/*!
*   @brief Interface for retreiving extended measurement object information
*/
interface ExtendedMeasurementObject : public MeasurementObject
{
    /*!
    *   @brief Method to retreive the property table of the measurement object.
    *   @return Return true if the measurement object has a property table, false otherwise.
    */
    virtual bool hasPropertyTable() = 0;

    /*!
    *  @brief Method used to insert a new entry to the property table.
    *  @param entryPair pair of strings containing the property name and the property value.
    *  @return Return true if the entry was inserted, false otherwise.
    */
    virtual bool insertEntry(const PropertyPair& entryPair) = 0;

    /*!
    *  @brief Method used to remove an entry from the property table.
    *  @param propertyName name of the property to be removed.
    *  @return Return true if the entry was removed, false otherwise.
    */
    virtual bool removeProperty(const std::string& propertyName) = 0;

    /*!
    *  @brief Method used to clear the property table.
    */
    virtual void clearPropertyTable() = 0;

    /*!
    *  @brief Method used to retreive the property table.
    *  @return Return the property table reference.
    */
    virtual const PropertyTable& getPropertyTable() = 0;

    /*!
    *  @brief Method used to retreive the value of a property.
    *  @param entry name of the property.
    *  @return Return the value of the property.
    */
    virtual const std::string& getPropertyEntryValue(const std::string& entry) = 0;
};

/*!
* @brief Interface for retreiving system object information
*/
interface SystemObject : public MeasurementObject
{
    /*!
    *  @brief Method used to initialize the system object.
    */
    virtual void initializeSystemObject() = 0;

    /*!
    *  @brief Method used to terminate the system object.
    */
    virtual void terminateSystemObject() = 0;
};

/*!
* @brief Interface for controlling the object lifecycle
*/
interface ObjectControl
{
    /*!
    *  @brief Method used to initialize the object.
    */
    virtual void initializeObject() = 0;

    /*!
    *  @brief Method used to terminate the object.
    */
    virtual void terminateObject() = 0;
};

/*!
* @brief Interface for controlling the gui object behaviour
*/
interface GuiControlIfc
{
    /*!
    *  @brief Method used to show the gui object.
    */
    virtual void show() = 0;

    /*!
    *  @brief Method used to hide the gui object.
    */
    virtual void hide() = 0;
};