#include "pch.h"
#include <RawDataProcessor.hpp>
#include <defs/GuiDefs.hpp>
#include <imgui_internal.h>

namespace processors
{
    RawDataProcessor::RawDataProcessor(InterfaceAccess* interfaceAccess, uint8_t nb, const std::string& name):
        interfaceAccess_(interfaceAccess),
        instanceNb_(nb),
        handle_(INVALID_HANDLE),
        name_(name + " # " + std::to_string(nb)),
        type_(MeasurementObjectType::data_receiver),
        showGui_(true),
        maxPkgInBuffer_(1024),
        maxPayloadSize_(64)
    {
        handle_ = (instanceNb_ + 1);
        handle_ = handle_ << 0x8;
    }
    RawDataProcessor::~RawDataProcessor()
    {
        interfaceAccess_ = nullptr;
        packagesBuffer_.clear();
    }

    const uint8_t& RawDataProcessor::getInstanceNumber()
    {
        return instanceNb_;
    }
    const uint64_t& RawDataProcessor::getHandle()
    {
        return handle_;
    }
    const MeasurementObjectType& RawDataProcessor::getType()
    {
        return type_;
    }
    const std::string& RawDataProcessor::getName()
    {
        return name_;
    }

    bool RawDataProcessor::validatePackage(DataPackageCPtr pkg)
    {
        std::lock_guard<std::mutex> lock(mtx_);
        packagesBuffer_.push_back(pkg);

        if(packagesBuffer_.size() > (size_t)maxPkgInBuffer_)
        {
            packagesBuffer_.erase(packagesBuffer_.begin(), packagesBuffer_.begin() +  (packagesBuffer_.size() - (size_t)maxPkgInBuffer_));
        }
        return true;
    }

    void* RawDataProcessor::getInterface(const std::string& interfaceName)
    {
        if(interfaceName == "ReceiverSinkManager")
        {
            return dynamic_cast<ReceiverSinkManager*>(this);
        }
        if(interfaceAccess_)
        {
            return interfaceAccess_->getInterface(interfaceName);
        }
        return nullptr;
    }

    bool RawDataProcessor::registerToReceiverSink(NotifySubjects* subject)
    {
        return subjects_.insert(subject).second;

    }
    bool RawDataProcessor::unregisterToReceiverSink(NotifySubjects* subject)
    {
        auto it = subjects_.find(subject);

        if(it == subjects_.end())
        {
            return false;
        }
        subjects_.erase(it);
        return true;
    }

    void RawDataProcessor::initializeObject()
    {
        subjects_.clear();
    }
    void RawDataProcessor::terminateObject()
    {
        subjects_.clear();
        delete this;
    }

    void RawDataProcessor::show(ImGuiContext* ctx)
    {
        ImGui::SetCurrentContext(ctx);

        if(ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("Show"))
            {
                if (ImGui::MenuItem(("Show "+ name_ +" MO").c_str(), "Ctrl+r")) { showGui_ = !showGui_; }
                
                ImGui::EndMenu();
            }
        }
        ImGui::EndMainMenuBar();

        
        ImGuiViewport* viewport = (ImGuiViewport*)(void*)ImGui::GetMainViewport();
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;
        if (ImGui::BeginViewportSideBar("##Objects toolbar", viewport, ImGuiDir_Right, RIGHT_BAR_WIDTH, window_flags))
        {
            if(ImGui::TreeNodeEx(name_.c_str(), ImGuiTreeNodeFlags_Framed))
            {
                ImGui::InputInt("Max packages", &maxPkgInBuffer_);
                ImGui::InputInt("Max payload", &maxPayloadSize_);
                ImGui::TreePop();
            }
                
            ImGui::End();
        }

        if(showGui_)
        {
            ImGui::Begin(name_.c_str(), &showGui_, ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.00f));
            if(ImGui::BeginTable("Raw Data Visualization", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollX |ImGuiTableFlags_ScrollY)){
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Timestamp");
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("Cycle");
            ImGui::TableSetColumnIndex(2);
            ImGui::Text("Type");
            ImGui::TableSetColumnIndex(3);
            ImGui::Text("Size");
            ImGui::TableSetColumnIndex(4);
            ImGui::Text("Data");

            std::lock_guard<std::mutex> lock(mtx_);
            for(auto const& pkg : packagesBuffer_)
            {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%" PRIu64, pkg->timestamp);
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%" PRIu8, pkg->cycle);
                ImGui::TableSetColumnIndex(2);
                ImGui::Text("%" PRIu8, (uint8_t)pkg->type);
                ImGui::TableSetColumnIndex(3);
                ImGui::Text("%" PRIu64, pkg->size);
                ImGui::TableSetColumnIndex(4);

                for(size_t i = 0; i < pkg->size && i < (size_t)maxPayloadSize_ ; i++)
                {
                    ImGui::Text("%02X", ((uint8_t*)pkg->payload)[i]); ImGui::SameLine();
                }

                if(pkg->size > (size_t)maxPayloadSize_)
                {
                    ImGui::Text("...");
                }
                
                
            }

            ImGui::EndTable();
            }
            ImGui::PopStyleColor();
            ImGui::End();
        }
    }
}