#include "DataVisualizer.hpp"

namespace application
{
    DataVisualizer::DataVisualizer(const std::string& name):
        name_(name),
        visible_(true),
        needConfiguration_(true),
        pkgHandle_(0),
        pkgTimestamp_(0),
        pkgSize_(0),
        registeredReceiverMgr_(nullptr),
        processorName_("None")
    {
        
    }

    void DataVisualizer::Show(std::shared_ptr<core::ConfigurationManager>& cfgMgr)
    {
        if (visible_)
        {
            ImGui::Begin(name_.c_str(), &visible_, ImGuiWindowFlags_AlwaysAutoResize);
            if(registeredReceiverMgr_ == nullptr)
            {
                if(ImGui::TreeNode("Connect to a data processor"))
                {
                    for(const auto& mo : cfgMgr->getMOsAddedInConfig())
                    {
                        if(mo->getType() == MeasurementObjectType::data_receiver)
                        {
                            processorName_ = mo->getName() + std::to_string(mo->getInstanceNumber());
                            if(ImGui::Button(processorName_.c_str()))
                            {
                                auto ifc = std::dynamic_pointer_cast<InterfaceAccess>(mo);
                                registeredReceiverMgr_ = static_cast<ReceiverSinkManager*>(ifc->getInterface("ReceiverSinkManager"));
                                registeredReceiverMgr_->registerToReceiverSink(this);
                                break;
                            }
                            processorName_ = "None";
                        }
                    }
                    ImGui::TreePop();
                }
            }
            else
            {
                if(ImGui::Button("Remove processor"))
                {
                    registeredReceiverMgr_->unregisterToReceiverSink(this);
                    registeredReceiverMgr_ = nullptr;
                    processorName_ = "None";
                }
            }
            ImGui::Text("Connected to processor: %s", processorName_.c_str());
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