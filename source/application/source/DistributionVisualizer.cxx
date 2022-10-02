#include "DistributionVisualizer.hpp"

namespace application
{
    DistributionVisualizer::DistributionVisualizer(const std::string& name, uint8_t instanceNb, DataDistributionStatistics* stat):
        name_(name),
        visible_(true),
        needConfiguration_(true),
        pkgHandle_(0),
        pkgTimestamp_(0),
        pkgSize_(0),
        dataDistributionStatPtr_(stat),
        processorName_("None"),
        instanceNb_(instanceNb),
        handle_(INVALID_HANDLE),
        type_(MeasurementObjectType::visualization)
    {
        handle_ = (instanceNb_ + 1);
        handle_ =  handle_ << 0x8 << 0x8 << 0x8 << 0x8 << 0x8;
        name_ = name_ + " #" + std::to_string(instanceNb_);
    }

    void DistributionVisualizer::Show(ConfigurationParser*)
    {
        if (visible_ && dataDistributionStatPtr_ != nullptr)
        {
            ImGui::Begin(name_.c_str(), &visible_, ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::Text("Maximum processed packages: %lu", dataDistributionStatPtr_->getMaximumProcessedPackagesPerSecond());
            ImGui::Text("Processed packages: %lu", dataDistributionStatPtr_->getNumberOfProcessedPackagesPerSecond());
           
            ImGui::End();
        }
    }

    void DistributionVisualizer::Reset()
    {
        dataDistributionStatPtr_ = nullptr;
    }

    const uint8_t& DistributionVisualizer::getInstanceNumber()
    {
        return instanceNb_;
    }
    const uint64_t& DistributionVisualizer::getHandle()
    {
        return handle_;
    }
    const MeasurementObjectType& DistributionVisualizer::getType()
    {
        return type_;
    }
    const std::string& DistributionVisualizer::getName()
    {
        return name_;
    }
}