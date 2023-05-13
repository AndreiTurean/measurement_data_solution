#include "pch.h"
#include <udp/UdpReceiver.hpp>
#include <Packet.h>
#include <PcapFilter.h>

void receivers::UdpReceiver::onPacketArrives(pcpp::RawPacket* packet, pcpp::PcapLiveDevice*, void* ifc)
{

    DataDistribution* dataDistributionInterface_ = reinterpret_cast<DataDistribution*>(ifc);
    
    auto pkg = std::make_shared<DataPackage>();
    pkg->timestamp = packet->getPacketTimeStamp().tv_sec * 1000000 + packet->getPacketTimeStamp().tv_nsec;
    pkg->size = packet->getRawDataLen();
    pkg->sourceHandle = 0XEAA8;
    pkg->cycle = 0;
    pkg->id = 0;
    pkg->type = PackageType::ethernet;
    pkg->payload = new uint8_t[pkg->size];
    std::memcpy(pkg->payload, packet->getRawData(), packet->getRawDataLen());
    dataDistributionInterface_->distributeData(pkg);
    
    
}

namespace receivers
{
    UdpReceiver::UdpReceiver(InterfaceAccess* interfaceAccess, const uint8_t instanceNb, const std::string& name)
    {
        interfaceAccess_ = interfaceAccess;
        instanceNumber_ = instanceNb;
        name_ = name + " # " +std::to_string(instanceNb);
        type_ = MeasurementObjectType::data_processor;
        dataDistributionInterface_ = reinterpret_cast<DataDistribution*>(interfaceAccess_->getInterface("DataDistribution"));
        handle_ = (instanceNb + 1);
        handle_ = handle_ << 0x8 << 0x8 << 0x8;
        loggingInterface_ = reinterpret_cast<LoggingInterface*>(interfaceAccess_->getInterface("LoggingInterface"));
        loggingInterface_->subscribe(name_.c_str(), handle_);
        ifcName_ = "None";
        currentDevice_ = nullptr;
        ipAdress_ = "0.0.0.0";
        
    }
    UdpReceiver::~UdpReceiver()
    {
        terminateObject();
        dataDistributionInterface_ = nullptr;
    }
    const uint8_t& UdpReceiver::getInstanceNumber()
    {
        return instanceNumber_;
    }
    const uint64_t& UdpReceiver::getHandle()
    {
        return handle_;
    }
    const MeasurementObjectType& UdpReceiver::getType()
    {
        return type_;
    }
    const std::string& UdpReceiver::getName()
    {
        return name_;
    }
    void UdpReceiver::initializeObject()
    {
        devList_ = pcpp::PcapLiveDeviceList::getInstance().clone();
    }
    void UdpReceiver::terminateObject()
    {
        devList_->reset();
    }
    void UdpReceiver::show(ImGuiContext* ctx)
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
                ImGui::Text("Receive eth frame from the following network interface");
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
                            auto filter = pcpp::IPFilter(ipAdress_, pcpp::Direction::SRC_OR_DST);
                            currentDevice_->setFilter(filter);
                            currentDevice_->startCapture(onPacketArrives, dataDistributionInterface_);
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