#include "pch.h"
#include <core/EngineObject.hpp>
#include <defs/GuiDefs.hpp>


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
        INIT_CONTEXT(ctx)
        
        BEGIN_MAIN_MENU_BAR
            BEGIN_MENU("Show")
                ADD_MENU_ITEM(showGui_ ? "Hide engine MO" : "Show engine MO", "Ctrl+w", showGui_)
            END_MENU
        END_MAIN_MENU_BAR

        if(showGui_)
        {
            BEGIN_GUI("Measurement objects", &showGui_, ImGuiWindowFlags_AlwaysAutoResize)
                BEGIN_TAB_BAR("Measurement objects")
                    ADD_TAB_ITEM(name_.c_str(), nullptr)
                        DISPLAY_MAP_ELEMENTS_STR(propertyTable_)
                    END_TAB_ITEM
                END_TAB_BAR
            END_GUI
        }
    }
}