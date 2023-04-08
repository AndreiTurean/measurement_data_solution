#pragma once
#include <mutex>
#include <memory>
#include <thread>
#include <defs/Log.hpp>
#include <defs/MdsInterface.hpp>

namespace core
{
    namespace metrics
    {
        class Watchdog : 
            public GuiControlIfc,
            public ExtendedMeasurementObject
        {
            LoggingInterface* logger_;
            std::unique_ptr<std::thread> watchThread_;
            bool alive_;
            uint64_t lastTimestamp_;
            uint64_t deltaTimestamp_;
            uint8_t instanceNumber_;
            uint64_t handle_;
            MeasurementObjectType type_;
            std::string name_;
            PropertyTable propertyTable_;
            std::mutex timestampGuard_;
            void watch();
        public:
            explicit Watchdog(LoggingInterface* loger);
            virtual ~Watchdog();
            Watchdog(const Watchdog& lhs) = delete;
            const Watchdog& operator=(const Watchdog& lhs) = delete;

            /*!
            *   @brief Get Watchdog measurement object instance number.
            *   @return engine instance number.
            */
            const uint8_t& getInstanceNumber() override;

            /*!
            *   @brief Get MDS engine measurement object handle.
            */
            const uint64_t& getHandle() override;

            /*!
            *   @brief Get MDS engine measurement object type.
            */
            const MeasurementObjectType& getType() override;

            /*!
            *   @brief Get MDS engine measurement object name.
            */
            const std::string& getName() override;

            bool hasPropertyTable() override;
            bool insertEntry(const PropertyPair& entryPair) override;
            bool removeProperty(const std::string& propertyName) override;
            void clearPropertyTable() override;
            const PropertyTable& getPropertyTable() override;
            const std::string& getPropertyEntryValue(const std::string& entry) override;

            /*!
            *  @brief Method used to show the gui object.
            */
            void show() override;

            /*!
            *  @brief Method used to hide the gui object.
            */
            void hide() override;
        };
    }
}