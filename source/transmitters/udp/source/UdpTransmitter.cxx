#include "pch.h"
#include <udp/UdpTransmitter.hpp>
#include <defs/GuiDefs.hpp>
#include <Packet.h>

namespace transmitters
{
    UdpTransmitter::UdpTransmitter(InterfaceAccess* interfaceAccess, uint8_t nb, const std::string& name)
    {
        interfaceAccess_ = interfaceAccess;
        instanceNb_ = nb;
        name_ = name + " # " + std::to_string(nb);
        type_ = MeasurementObjectType::data_receiver;
        handle_ = (nb + 1);
        handle_ = handle_ << 0x8 << 0x8 << 0x8;
        loggingInterface_ = reinterpret_cast<LoggingInterface*>(interfaceAccess_->getInterface("LoggingInterface"));
        loggingInterface_->subscribe(name_.c_str(), handle_);
        maxPkgInBuffer_ = 100;
        currentDevice_ = nullptr;
        ifcName_ = "None";
        ipAdress_ = "192.168.1.255";
    }

    UdpTransmitter::~UdpTransmitter()
    {
        terminateObject();
    }

    //! MeasurementObject interface implementation
    const uint8_t& UdpTransmitter::getInstanceNumber()
    {
        return instanceNb_;
    }
    const uint64_t& UdpTransmitter::getHandle()
    {
        return handle_;
    }
    const MeasurementObjectType& UdpTransmitter::getType()
    {
        return type_;
    }
    const std::string& UdpTransmitter::getName()
    {
        return name_;
    }

    //! DataReceiverObject interface implementation
    bool UdpTransmitter::validatePackage(DataPackageCPtr package)
    {
        if(!currentDevice_)
        {
            packagesBuffer_.push_back(package);
        }
        else
        {
            timeval ts;
            ts.tv_sec = package->timestamp / 1000000000;
            ts.tv_usec = package->timestamp % 1000000000;
            pcpp::RawPacket packet((uint8_t*)package->payload, package->size, ts, false);
            pcpp::Packet parsedPacket(&packet);
            loggingInterface_->log(parsedPacket.toString().c_str(), handle_, severity::debug);
            return currentDevice_->sendPacket(packet);
        }
        return false;
    }

    void UdpTransmitter::initializeObject()
    {
        devList_ = pcpp::PcapLiveDeviceList::getInstance().clone();
    }
    void UdpTransmitter::terminateObject()
    {
        devList_->reset();
    }

    void UdpTransmitter::show(ImGuiContext* ctx)
    {
        ImGui::SetCurrentContext(ctx);
        ImGuiViewport* viewport = (ImGuiViewport*)(void*)ImGui::GetMainViewport();
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;
        if (ImGui::BeginViewportSideBar("##Objects toolbar", viewport, ImGuiDir_Right, RIGHT_BAR_WIDTH, window_flags))
        {
            if (ImGui::TreeNodeEx(name_.c_str(), ImGuiTreeNodeFlags_Framed))
            {
                std::string tempIpAdress_ = "0.0.0.0";

                if(ImGui::InputText("Ip adress filter", &tempIpAdress_, ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    ipAdress_ = tempIpAdress_;
                }

                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
                ImGui::Text("Ip adress filter: %s", ipAdress_.c_str());
                ImGui::PopStyleColor();

                ImGui::Text("Transmit eth frame from the following network interface");
                if(ImGui::BeginCombo("Network", ifcName_.c_str()))
                {
                    for(pcpp::PcapLiveDevice* dev : devList_->getPcapLiveDevicesList())
                    {
                        
                        if (ImGui::Selectable(dev->getName().c_str()))
                        {
                            currentDevice_ = dev;
                            if(!currentDevice_->open())
                            {
                                loggingInterface_->log("Failed to open device", handle_ , severity::critical);
                            }

                            ifcName_ = dev->getName();
                        }
                    }
                    if(ImGui::Selectable("None") && currentDevice_ != nullptr)
                    {
                        ifcName_ = "None";
                        currentDevice_->stopCapture();
                        currentDevice_ = nullptr;
                    }
                    ImGui::EndCombo();
                }
                ImGui::TreePop();
            }
            ImGui::End();
        }
    }
}