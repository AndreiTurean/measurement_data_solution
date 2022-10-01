#pragma once
#include <defs/MdsInterface.hpp>

namespace core
{
    /*!
    *   @brief MDS engine measurement object
    */
    class EngineObject:
        public ExtendedMeasurementObject
    {
        uint8_t instanceNumber_; //!< engine instance number. Shall be 0
        uint64_t handle_; //!< engine handle. Shall be 0
        std::string name_; //!< engine name
        MeasurementObjectType type_; //!< engine type.
        PropertyTable propertyTable_;
    public:
        /*!
        *   @brief MDS engine measurement object constructor.
        */
        EngineObject();

        /*!
        *   @brief Get MDS engine measurement object instance number.
        *   @return engine instance number.
        */
        virtual const uint8_t& getInstanceNumber() override;

        /*!
        *   @brief Get MDS engine measurement object handle.
        */
        virtual const uint64_t& getHandle() override;

        /*!
        *   @brief Get MDS engine measurement object type.
        */
        virtual const MeasurementObjectType& getType() override;

        /*!
        *   @brief Get MDS engine measurement object name.
        */
        virtual const std::string& getName() override;

        virtual bool hasPropertyTable() override;
        virtual bool insertEntry(const PropertyPair& entryPair) override;
        virtual bool removeProperty(const std::string& propertyName) override;
        virtual void clearPropertyTable() override;
        virtual const PropertyTable& getPropertyTable() override;
        virtual const std::string& getPropertyEntryValue(const std::string& entry) override;

    };
}