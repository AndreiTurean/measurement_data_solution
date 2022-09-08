#include <Logger.hpp>
#include <iostream>
#include <chrono>

namespace core
{
    Logger::Logger(InterfaceAccess* interfaceAccess, bool ignoreDebug):
        interfaceAccess_(interfaceAccess),
        ignoreDebugMsg_(ignoreDebug)
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
        switch (sev)
        {
        case severity::debug:
        {
            if(!ignoreDebugMsg_)
                std::cout<< std::to_string(timestamp) <<" [DBG/"<< logRegister_.getName(handle) << "]: "<< message << std::endl;
        }
        break;
        case severity::information:
        {
            std::cout<< std::to_string(timestamp) << "[INFO/"<< logRegister_.getName(handle) << "]: "<< message << std::endl;
        }
        break;
        case severity::warning:
        {
            std::cout<< std::to_string(timestamp) <<"[WARN/"<< logRegister_.getName(handle) << "]: "<< message << std::endl;
        }
        break;
        case severity::error:
        {
            std::cerr<< std::to_string(timestamp) <<"[ERR/"<< logRegister_.getName(handle) << "]: "<< message << std::endl;
        }
        break;
        case severity::critical:
        {
            std::cerr<< std::to_string(timestamp) <<"[CRIT/"<< logRegister_.getName(handle) << "]: "<< message << std::endl;
        }
        break;
        default:
            break;
        }
    }
    bool Logger::subscribe(const char* name, const uint64_t handle)
    {
        return logRegister_.insertHandle(handle, name);
    }
    bool Logger::unsubscribe(const uint64_t handle)
    {
        return logRegister_.removeHandle(handle);
    }
}