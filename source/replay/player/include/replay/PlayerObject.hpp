#pragma once

// STL headers
#include <string>

// Core headers
#include <defs/MdsInterface.hpp>
#include <defs/MeasurementObjectDefs.hpp>

// Replay headers
#include <PlayerDefs.hpp>
#include <replay/Player.hpp>

namespace replay
{
    class REPLAY_API PlayerObject :
        public InterfaceAccess,
        public MeasurementObject,
        public GuiControlIfc

    {
    public:
        PlayerObject(InterfaceAccess* interfaceAccess, const uint8_t& instanceNb, const std::string& name);
        //virtual ~PlayerObject();
        //! MeasurementObject interface implementation
        const uint8_t& getInstanceNumber();
        const uint64_t& getHandle();
        const MeasurementObjectType& getType();
        const std::string& getName();

        //! InterfaceAccess interface implementation
        void* getInterface(const std::string& interfaceName);

        //! GuiControlIfc interface implementation
        void show(ImGuiContext* ctx) override;

    private:
        InterfaceAccess* interfaceAccess_;
        uint8_t instanceNumber_;
        uint64_t handle_;
        MeasurementObjectType measurementObjectType_;
        std::string name_;
        Player player_; 
        bool openRecordingFile_;
        bool showGui_;
    };
}

extern "C" REPLAY_API MeasurementObjectPtr createMO(InterfaceAccess* interfaceAccess, const uint8_t instanceNb, const char* name)
{
    return new replay::PlayerObject(interfaceAccess, instanceNb, name);
}

