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
            std::shared_ptr<LoggingInterface> logger_;
            std::unique_ptr<std::thread> watchThread_;
            bool alive_;
            uint64_t lastTimestamp_;
            void watch();
        public:
            explicit Watchdog(std::shared_ptr<LoggingInterface> loger);
            ~Watchdog();
            Watchdog(const Watchdog& lhs) = delete;
            const Watchdog& operator=(const Watchdog& lhs) = delete;
        };
    }
}