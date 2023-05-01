#include "DataVisualizer.hpp"

namespace application
{
    DataVisualizer::DataVisualizer(const std::string& name, uint8_t instanceNb):
        name_(name),
        visible_(true),
        needConfiguration_(true),
        pkgHandle_(0),
        pkgTimestamp_(0),
        pkgSize_(0),
        registeredReceiverMgr_(nullptr),
        processorName_("None"),
        instanceNb_(instanceNb),
        handle_(INVALID_HANDLE),
        type_(MeasurementObjectType::visualization)
    {
        handle_ = (instanceNb_ + 1);
        handle_ =  handle_ << 0x8 << 0x8 << 0x8 << 0x8 << 0x8;
        name_ = name_ + " #" + std::to_string(instanceNb_);
    }

    void DataVisualizer::Show(ConfigurationParser* cfgMgr)
    {
        if (visible_)
        {
            ImGui::Begin(name_.c_str(), &visible_, ImGuiWindowFlags_AlwaysAutoResize);
            if(registeredReceiverMgr_ == nullptr)
            {
                if(ImGui::TreeNodeEx("Connect to a data processor", ImGuiTreeNodeFlags_Framed))
                {
                    for(const auto& mo : cfgMgr->getMOsAddedInConfig())
                    {
                        if(mo->getType() == MeasurementObjectType::data_receiver)
                        {
                            processorName_ = mo->getName() + std::to_string(mo->getInstanceNumber());

                            if(ImGui::Button(processorName_.c_str()))
                            {
                                auto ifc = dynamic_cast<InterfaceAccess*>(mo);
                                registeredReceiverMgr_ = static_cast<ReceiverSinkManager*>(ifc->getInterface("ReceiverSinkManager"));

                                if(!registeredReceiverMgr_)
                                {
                                    throw std::runtime_error("Failed to get ReceiverSinkManager interface");
                                }
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

    void DataVisualizer::Reset()
    {
        registeredReceiverMgr_ = nullptr;
    }

    bool DataVisualizer::notify(DataPackageCPtr pkg)
    {
        pkgHandle_ = pkg->sourceHandle;
        pkgTimestamp_ = pkg->timestamp;
        pkgSize_ = pkg->size;
        return true;
    }

    const uint8_t& DataVisualizer::getInstanceNumber()
    {
        return instanceNb_;
    }
    const uint64_t& DataVisualizer::getHandle()
    {
        return handle_;
    }
    const MeasurementObjectType& DataVisualizer::getType()
    {
        return type_;
    }
    const std::string& DataVisualizer::getName()
    {
        return name_;
    }
}