#pragma once
#include <defs/MeasurementObjectDefs.hpp>
#include <statistics/CPUHelper.hpp>
#include <array>


namespace core
{
    class CPUMonitor:
        public GuiControlIfc
    {
        static constexpr uint16_t HISTORY_SIZE = 1024;
        core::helper::CPUHelper cpuHelper_;
        std::array<float, HISTORY_SIZE> cpuHistory_;
        uint16_t currentIdx_;
        
    public:
        CPUMonitor();
        ~CPUMonitor() = default;
        void show(ImGuiContext* ctx) override;
    };
}