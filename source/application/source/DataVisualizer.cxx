#include "DataVisualizer.hpp"

namespace application
{
    DataVisualizer::DataVisualizer(const std::string& name):
        name_(name),
        visible_(true),
        needConfiguration_(true),
        pkgHandle_(0),
        pkgTimestamp_(0),
        pkgSize_(0)
    {
        
    }

    void DataVisualizer::Show(std::shared_ptr<core::ConfigurationManager>& cfgMgr)
    {
        if (visible_)
        {
            ImGui::Begin(name_.c_str(), &visible_, ImGuiWindowFlags_AlwaysAutoResize);
            if(needConfiguration_)
            {
            if(ImGui::TreeNode("Connect to a data receiver"))
            {
                    for(const auto& mo : cfgMgr->getMOsAddedInConfig())
                    {
                        if(mo->getType() == MeasurementObjectType::data_receiver)
                        {
                            if(ImGui::Button(mo->getName().c_str()))
                            {
                                auto ifc = std::dynamic_pointer_cast<InterfaceAccess>(mo);
                                ReceiverSinkManager* mgr = static_cast<ReceiverSinkManager*>(ifc->getInterface("ReceiverSinkManager"));
                                needConfiguration_ = !mgr->registerToReceiverSink(this);
                            }
                        }
                    }
                    ImGui::TreePop();
                }
            }
            ImGui::Text("Last received package size: %zu", pkgSize_);
            ImGui::Text("Last received package timestamp: %lu", pkgTimestamp_);
            ImGui::Text("Last received package handle: %lu", pkgHandle_);
            ImGui::End();
        }
    }

    bool DataVisualizer::notify(DataPackageCPtr pkg)
    {
        pkgHandle_ = pkg->sourceHandle;
        pkgTimestamp_ = pkg->timestamp;
        pkgSize_ = pkg->size;
        return true;
    }
}