#pragma once
#include <defs/Log.hpp>
#include <defs/MdsInterface.hpp>
#include <LogRegistry.hpp>
#include <mutex>
#include <vector>

namespace core
{
    class Logger:
        public InterfaceAccess,
        public LoggingInterface,
        public GuiControlIfc
    {
        InterfaceAccess* interfaceAccess_;
        bool ignoreDebugMsg_;
        std::mutex loggingGuard_;
        std::vector<std::string> logBuffer_;
    public:
        Logger(InterfaceAccess* interfaceAccess, bool ignoreDebug = false);
        virtual ~Logger();
        void* getInterface(const std::string& interfaceName) override;
        void save() override;
        void log(const char* message,const uint64_t handle  = 0, const severity sev = severity::debug) override;
        bool subscribe(const char* name = "Engine", const uint64_t handle = 0) override;
        bool unsubscribe(const uint64_t handle = 0) override;

        void show() override;
        void hide() override;

    private:
        LogRegister logRegister_;
    };
}