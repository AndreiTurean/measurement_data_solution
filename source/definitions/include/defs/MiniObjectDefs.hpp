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
using MeasurementObjectPtr = std::shared_ptr<MeasurementObject>;
//! List of measurement objects
using MeasurementObjectList = std::list<std::shared_ptr<MeasurementObject>>;

enum class PackageType : uint8_t
{
    dummy = 0x00,
    camera = 0x01,
    can = 0x02,
    flexray = 0x04,
    ethernet = 0x08
};
/*!
*   @brief Data package definition
*/
struct DataPackage
{
    uint64_t timestamp; //!< package timestamp
    uint64_t sourceHandle; //!< package source handle
    size_t size; //!< package size
    uint8_t cycle_; //!< package cycle
    PackageType type; //!< package type
    void* payload; //!< pointer to where the package payload starts.
};
//! Data package pointer
using DataPackagePtr = std::shared_ptr<DataPackage>;
//! Const data package pointer
using DataPackageCPtr = std::shared_ptr<const DataPackage>;

/*!
*   @brief Interface used by any processor in order to receive packages from the distribution manager
*/
struct DataReceiverObject
{
    /*!
    *   @brief Process data package.
    *   @note The package cannot be altered, but a new package can be created to be delivered to the subscribers
    *   @param package Pointer to a const data package that will be analyzed and proccessed.
    *   @return Return true if all the processors validate the package, false if any proccesor cannot validate it.
    */
    virtual bool validatePackage(DataPackageCPtr package) = 0;
};

//! Data processor pointer
using DataReceiverObjectPtr = std::shared_ptr<DataReceiverObject>;

/*!
*   @brief Interface used by any data transmitter in order to transmit packages to the distribution manager
*/
struct DataSenderObject
{
    /*!
    *   @brief start the processing threah that will distribute data to the data distribution manager
    */
    virtual void startProcessing() = 0;
    /*!
    *   @brief end the processing threah that will distribute data to the data distribution manager
    */
    virtual void endProcessing() = 0;
};

//! Data transmitter pointer
using DataSenderObjectPtr = std::shared_ptr<DataSenderObject>;

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