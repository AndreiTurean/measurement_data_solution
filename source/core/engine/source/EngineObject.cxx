#include "pch.h"
#include <core/EngineObject.hpp>


namespace core
{
    EngineObject::EngineObject():
        instanceNumber_(0),
        handle_(ENGINE_HANDLE),
        name_("Engine"),
        type_(MeasurementObjectType::system)
    {
        propertyTable_.insert(std::make_pair("Name", "Engine"));
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

    void EngineObject::show()
    {
        ImGui::Begin(name_.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize);

        for(const auto& entry : propertyTable_)
        {
            ImGui::Text("%s : %s", entry.first.c_str(), entry.second.c_str());
        }

        ImGui::End();
    }

    void EngineObject::hide()
    {

    }
}