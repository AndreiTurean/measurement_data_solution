#pragma once
#include <memory>
#include <thread>
#include <defs/Log.hpp>

namespace core
{
    namespace metrics
    {
        class Watchdog
        {
            LoggingInterface* logger_;
            std::unique_ptr<std::thread> watchThread_;
            bool alive_;
            uint64_t lastTimestamp_;
            void watch();
        public:
            explicit Watchdog(LoggingInterface* loger);
            virtual ~Watchdog();
            Watchdog(const Watchdog& lhs) = delete;
            const Watchdog& operator=(const Watchdog& lhs) = delete;
        };
    }
}