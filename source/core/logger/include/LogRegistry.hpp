#pragma once
#include <map>
#include <cstdint>

namespace core
{
    class LogRegister
    {
        std::map<uint64_t, const char*> register_;
    public:
        LogRegister();
        bool insertHandle(const uint64_t handle, const char* name);
        bool removeHandle(const uint64_t handle);
        const char* getName(const uint64_t handle);
    };
}