#include "pch.h"
#include <RawDataProcessor.hpp>

namespace processors
{
    RawDataProcessor::RawDataProcessor(InterfaceAccess* interfaceAccess, uint8_t nb, const std::string& name):
        interfaceAccess_(interfaceAccess),
        instanceNb_(nb),
        handle_(INVALID_HANDLE),
        name_(name),
        type_(MeasurementObjectType::data_receiver),
        showGui_(false),
        maxPkgInBuffer_(1024)
    {
        handle_ = (instanceNb_ + 1);
        handle_ = handle_ << 0x8;
    }
    RawDataProcessor::~RawDataProcessor()
    {
        interfaceAccess_ = nullptr;
        subjects_.clear();
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

        if(showGui_)
        {
            ImGui::Begin("MOs", &showGui_, ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::BeginTabBar("MOs", ImGuiTabBarFlags_None);
            if(ImGui::BeginTabItem(name_.c_str(), nullptr, ImGuiTabItemFlags_None))
            {
                ImGui::InputInt("Max packages in buffer", &maxPkgInBuffer_);
                // for(const auto& entry : propertyTable_)
                // {
                //     ImGui::Text("%s : %s", entry.first.c_str(), entry.second.c_str());
                // }
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
            ImGui::End();

            ImGui::Begin("RawDataProcessor", &showGui_, ImGuiWindowFlags_AlwaysAutoResize);

            ImGui::Text("Timestamp, \tCycle, \tType, \tSize, \tData");
            std::lock_guard<std::mutex> lock(mtx_);
            for(auto const& pkg : packagesBuffer_)
            {
                std::string msg((char*)pkg->payload, pkg->size);
                ImGui::Text("%" PRIu64 "\t %" PRIu8 "\t %" PRIu8 "\t %" PRIu64 "\t %s", pkg->timestamp, pkg->cycle_, (uint8_t)pkg->type, pkg->size, msg.c_str());
            }
            ImGui::End();
        }
    }
}