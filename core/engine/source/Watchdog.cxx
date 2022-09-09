#include "Watchdog.hpp"
#include <chrono>
#include <cstdint>

namespace core
{
    namespace metrics
    {
        using namespace std::chrono_literals;
        void Watchdog::watch()
        {
            while(alive_)
            {
                uint64_t timestamp = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    
                if(lastTimestamp_ != 0 && timestamp - lastTimestamp_ > 2000)
                    logger_->log("Timegap detected. Possible hardware deadlock.", 3, severity::warning);

                lastTimestamp_ = timestamp;
                std::this_thread::sleep_for(1ms);
            }
        }

        Watchdog::Watchdog(std::shared_ptr<LoggingInterface> logger):
            logger_(logger),
            lastTimestamp_(0)
        {
            watchThread_ = std::make_unique<std::thread>(&Watchdog::watch, this);
            logger_->subscribe("Watchdog", 3);
        }
        Watchdog::~Watchdog()
        {
            alive_ = false;
            watchThread_->join();
            watchThread_.reset();
        }
    }
}