#include "pch.h"
#include <replay/PlayerObject.hpp>

namespace replay
{
    PlayerObject::PlayerObject(InterfaceAccess* interfaceAccess , const uint8_t& instanceNb, const std::string& name):
        interfaceAccess_(interfaceAccess),
        instanceNumber_(instanceNb),
        handle_(INVALID_HANDLE),
        measurementObjectType_(MeasurementObjectType::player),
        name_(name),
        player_(interfaceAccess)
    {

    }

    const uint8_t& PlayerObject::getInstanceNumber()
    {
        return instanceNumber_;
    }

    const uint64_t& PlayerObject::getHandle()
    {
        return handle_;
    }

    const MeasurementObjectType& PlayerObject::getType()
    {
        return measurementObjectType_;
    }

    const std::string& PlayerObject::getName()
    {
        return name_;
    }

    void* PlayerObject::getInterface(const std::string&)
    {
        return nullptr;
    }

    void PlayerObject::show(ImGuiContext* ctx)
    {
        ImGui::SetCurrentContext(ctx);
        if(ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open recording file", "Ctrl+r")) { openRecordingFile_ = true; }
                
                ImGui::EndMenu();
            }
        }
        ImGui::EndMainMenuBar();

        ImGui::SetCurrentContext(ctx);
        if(ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("Show"))
            {
                if (ImGui::MenuItem("Show player MO", "Ctrl+p")) { showGui_ = !showGui_; }
                
                ImGui::EndMenu();
            }
        }
        ImGui::EndMainMenuBar();

        player_.show(ctx);
    }
}