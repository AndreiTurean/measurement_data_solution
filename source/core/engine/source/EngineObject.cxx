#include "pch.h"
#include <core/EngineObject.hpp>


namespace core
{
    EngineObject::EngineObject():
        instanceNumber_(0),
        handle_(ENGINE_HANDLE),
        name_("Engine"),
        type_(MeasurementObjectType::system),
        showGui_(false)
    {
        propertyTable_.insert(std::make_pair("Name", "Engine"));
        propertyTable_.insert(std::make_pair("Type", "System"));
        propertyTable_.insert(std::make_pair("Handle", std::to_string(ENGINE_HANDLE)));
        propertyTable_.insert(std::make_pair("Instance", std::to_string(instanceNumber_)));
    }

    const uint8_t& EngineObject::getInstanceNumber()
    {
        return instanceNumber_;
    }
    const uint64_t& EngineObject::getHandle()
    {
        return handle_;
    }
    const MeasurementObjectType& EngineObject::getType()
    {
        return type_;
    }
    const std::string& EngineObject::getName()
    {
        return name_;
    }

    EngineObject::~EngineObject()
    {
        
    }

    bool EngineObject::hasPropertyTable()
    {
        return !propertyTable_.empty();
    }
    bool EngineObject::insertEntry(const PropertyPair& entryPair)
    {
        return propertyTable_.insert(entryPair).second;
    }
    bool EngineObject::removeProperty(const std::string& propertyName)
    {
        auto it = propertyTable_.find(propertyName);
        
        if(it == propertyTable_.end())
        {
            return false;
        }

        propertyTable_.erase(it);

        return true;
    }
    void EngineObject::clearPropertyTable()
    {
        propertyTable_.clear();
    }
    const PropertyTable& EngineObject::getPropertyTable()
    {
        return propertyTable_;
    }
    const std::string& EngineObject::getPropertyEntryValue(const std::string& entry)
    {
        return propertyTable_[entry];
    }

    void EngineObject::show(ImGuiContext* ctx)
    {
        ImGui::SetCurrentContext(ctx);
        if(ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("Show"))
            {
                if (ImGui::MenuItem("Show engine MO", "Ctrl+w")) { showGui_ = !showGui_; }
                
                ImGui::EndMenu();
            }
        }
        ImGui::EndMainMenuBar();

        if(showGui_)
        {
            ImGui::Begin("MOs", &showGui_, ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::BeginTabBar("MOs", ImGuiTabBarFlags_None);
            if(ImGui::BeginTabItem(name_.c_str(), nullptr, ImGuiTabItemFlags_None))
            {
                for(const auto& entry : propertyTable_)
                {
                    ImGui::Text("%s : %s", entry.first.c_str(), entry.second.c_str());
                }
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
            ImGui::End();
        }
    }

    void EngineObject::hide()
    {
        showGui_ = false;
    }
}