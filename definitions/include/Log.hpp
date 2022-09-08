#pragma once
#include <cstdint>

enum class severity : uint8_t
{
    debug = 0x00,
    information = 0x01,
    warning = 0x02,
    error = 0x03,
    critical = 0x04
};

struct LoggingInterface
{
    virtual void save() = 0;
    virtual void log(const char* message,const uint64_t handle  = 0, const severity sev = severity::debug) = 0;
    virtual bool subscribe(const char* name = "Engine", const uint64_t handle = 0) = 0;
    virtual bool unsubscribe(const uint64_t handle = 0) = 0;
};