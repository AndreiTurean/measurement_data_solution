#include "pch.h"
#include <Logger.hpp>

#include <iostream>
#include <chrono>
#include <sstream>
#include <algorithm>

#include <defs/GuiDefs.hpp>

namespace core
{
    Logger::Logger(InterfaceAccess* interfaceAccess, bool ignoreDebug):
        interfaceAccess_(interfaceAccess),
        ignoreDebugMsg_(ignoreDebug),
        logRegister_(this),
        showAbout_(false),
        maximumLogSize_(100)
    {
    }
    void* Logger::getInterface(const std::string& interfaceName)
    {
        if(interfaceName == "LoggingInterface")
        {
            return dynamic_cast<LoggingInterface*>(this);
        }
        if(interfaceAccess_)
        {
            return interfaceAccess_->getInterface(interfaceName);
        }
        return nullptr;
    }
    void Logger::save()
    {

    }
    void Logger::log(const char* message,const uint64_t handle, const severity sev)
    {
        std::lock_guard<std::mutex> lock(loggingGuard_);
        uint64_t timestamp = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

        std::stringstream logMessageStream;

        switch (sev)
        {
        case severity::debug:
        {
            logMessageStream << std::to_string(timestamp) <<" [DBG/"<< logRegister_.getName(handle) << "]: "<< message << std::endl;
            if(!ignoreDebugMsg_)
                std::cout<< logMessageStream.str();
        }
        break;
        case severity::information:
        {
            logMessageStream << std::to_string(timestamp) << " [INFO/"<< logRegister_.getName(handle) << "]: "<< message << std::endl;
            std::cout << logMessageStream.str();
        }
        break;
        case severity::warning:
        {
            logMessageStream << std::to_string(timestamp) <<" [WARN/"<< logRegister_.getName(handle) << "]: "<< message << std::endl;
            std::cout << logMessageStream.str();
        }
        break;
        case severity::error:
        {
            logMessageStream << std::to_string(timestamp) <<" [ERR/"<< logRegister_.getName(handle) << "]: "<< message << std::endl;
            std::cerr << logMessageStream.str();
        }
        break;
        case severity::critical:
        {
            logMessageStream << std::to_string(timestamp) <<" [CRIT/"<< logRegister_.getName(handle) << "]: "<< message << std::endl;
            std::cerr << logMessageStream.str();
        }
        break;
        default:
            break;
        }
        if(logBuffer_.size() >= (size_t)maximumLogSize_)
        {
            logBuffer_.erase(logBuffer_.begin(), logBuffer_.begin() + (logBuffer_.size() - (size_t)maximumLogSize_));
        }
        logBuffer_.push_back(logMessageStream.str());
    }
    bool Logger::subscribe(const char* name, const uint64_t handle)
    {
        return logRegister_.insertHandle(handle, name);
    }
    bool Logger::unsubscribe(const uint64_t handle)
    {
        return logRegister_.removeHandle(handle);
    }

    void Logger::show(ImGuiContext* ctx)
    {
        INIT_CONTEXT(ctx)

        static auto color = [&](auto msg)
        {
            if(msg.find("[DBG") != std::string::npos)
            {
                return ImVec4(0.0f, 1.0f, 0.1f, 0.3f);
            }
            if(msg.find("[INFO") != std::string::npos)
            {
                return ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
            }
            if(msg.find("[WARN") != std::string::npos)
            {
                return ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
            }
            if(msg.find("[ERR") != std::string::npos)
            {
                return ImVec4(1.0f, 0.3f, 0.0f, 1.0f);
            }
            if(msg.find("[CRIT") != std::string::npos)
            {
                return ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
            }

            return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        };

        std::lock_guard<std::mutex> lock(loggingGuard_);

        BEGIN_GUI("Log monitor", nullptr, ImGuiWindowFlags_AlwaysAutoResize)
            DISPLAY_VECTOR_ELEMENTS_STR(logBuffer_, color)
        END_GUI

        BEGIN_MAIN_MENU_BAR
            BEGIN_MENU("About")
                ADD_MENU_ITEM("About logger", "Ctrl+e", showAbout_)
            END_MENU
        END_MAIN_MENU_BAR

        DISPLAY_ABOUT_MENU(showAbout_, "About logger")
    }
}