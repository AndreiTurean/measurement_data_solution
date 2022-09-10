#include <LogRegistry.hpp>
#include <string>

namespace core
{
    LogRegister::LogRegister()
    {
        register_[0] = "Engine";
    }
    bool LogRegister::insertHandle(const uint64_t handle, const char* name)
    {
        auto iter = register_.find(handle);

        if(iter != register_.end())
        {
            return false;
        }
        register_[handle] = name;
        return true;
    }
    bool LogRegister::removeHandle(const uint64_t handle)
    {
        auto iter = register_.find(handle);

        if(iter == register_.end())
        {
            return false;
        }

        register_.erase(iter);
        return true;
    }

    const char* LogRegister::getName(const uint64_t handle)
    {
        auto iter = register_.find(handle);

        if(iter == register_.end())
        {
            return std::to_string(handle).c_str();
        }

        return register_[handle];
    }
}