#include "pch.h"
#include <chrono>
#include <cstdint>
#include "Watchdog.hpp"
#include <defs/MdsInterface.hpp>
#include <inttypes.h>

namespace core
{
    namespace metrics
    {
        using namespace std::chrono_literals;
        void Watchdog::watch()
        {
            while(alive_)
            {
                {
                    std::lock_guard<std::mutex> lock(timestampGuard_);
                    uint64_t timestamp = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                    deltaTimestamp_ = timestamp - lastTimestamp_;

                    if(lastTimestamp_ != 0 && deltaTimestamp_ > (uint64_t)maxDuration_)
                        logger_->log(("Timegap of:" + std::to_string(deltaTimestamp_) + " was detected. Possible hardware deadlock.").c_str(), 3, severity::warning);

                    lastTimestamp_ = timestamp;
                }

                std::this_thread::sleep_for(1ms);
            }
        }

        Watchdog::Watchdog(LoggingInterface* logger):
            logger_(logger),
            alive_(true),
            lastTimestamp_(0),
            deltaTimestamp_(0),
            instanceNumber_(0),
            handle_(WATCHDOG_HANDLE),
            type_(MeasurementObjectType::system),
            name_("Watchdog"),
            showWatchdog_(false),
            maxDuration_(0xffff)
        {
            watchThread_ = std::make_unique<std::thread>(&Watchdog::watch, this);
            logger_->subscribe("Watchdog", WATCHDOG_HANDLE);
            logger_->log("Watchdog started", WATCHDOG_HANDLE, severity::information);
        }
        Watchdog::~Watchdog()
        {
            logger_->log("Watchdog stopped", WATCHDOG_HANDLE, severity::information);
            logger_->unsubscribe(WATCHDOG_HANDLE);
            alive_ = false;
            watchThread_->join();
            watchThread_.reset();
        }

        void Watchdog::show(ImGuiContext* ctx)
        {
            ImGui::SetCurrentContext(ctx);
            if(ImGui::BeginMainMenuBar())
            {
                if (ImGui::BeginMenu("Show"))
                {
                    if (ImGui::MenuItem("Show watchdog MO", "Ctrl+w")) { showWatchdog_ = !showWatchdog_; }
                    
                    ImGui::EndMenu();
                }
            }
            ImGui::EndMainMenuBar();

            if (showWatchdog_)
            {
                std::lock_guard<std::mutex> lock(timestampGuard_);
                ImGui::Begin("MOs", &showWatchdog_, ImGuiWindowFlags_AlwaysAutoResize);
                ImGui::BeginTabBar("MOs", ImGuiTabBarFlags_None);
                if(ImGui::BeginTabItem(name_.c_str(), nullptr, ImGuiTabItemFlags_None))
                {
                    ImGui::Text("Last timestamp: %" PRIu64, lastTimestamp_);
                    ImGui::Text("Delta timestamp (microseconds): %" PRIu64, deltaTimestamp_);
                    ImGui::SliderInt("Max duration",&maxDuration_, 0, 0xffff);
                    ImGui::EndTabItem();
                }

                ImGui::EndTabBar();

                ImGui::End();
            }
        }

        void Watchdog::hide()
        {
            // Do nothing
        }

        const uint8_t& Watchdog::getInstanceNumber()
        {
            return instanceNumber_;
        }

        const uint64_t& Watchdog::getHandle()
        {
            return handle_;
        }

        const MeasurementObjectType& Watchdog::getType()
        {
            return type_;
        }

        const std::string& Watchdog::getName()
        {
            return name_;
        }

        bool Watchdog::hasPropertyTable()
        {
            return true;
        }

        bool Watchdog::insertEntry(const PropertyPair& entryPair)
        {
            return propertyTable_.insert(entryPair).second;
        }

        bool Watchdog::removeProperty(const std::string& propertyName)
        {
            return propertyTable_.erase(propertyName) > 0;
        }

        void Watchdog::clearPropertyTable()
        {
            propertyTable_.clear();
        }
        const PropertyTable& Watchdog::getPropertyTable()
        {
            return propertyTable_;
        }
        const std::string& Watchdog::getPropertyEntryValue(const std::string& entry)
        {
            return propertyTable_.at(entry);
        }
    }
}