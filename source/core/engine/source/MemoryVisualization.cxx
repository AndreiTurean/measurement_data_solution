#include "pch.h"
#include "MemoryVisualization.hpp"
#include <statistics/MemoryUsageStatistics.hpp>

namespace core
{
    void MemoryVisualization::show(ImGuiContext* ctx)
    {
        auto rssMem = core::statistics::getCurrentRSS();
        auto peakRSS = core::statistics::getPeakRSS();
        auto totalMemory = core::statistics::getTotalMemoryAvailable();

        rssMem /= 1024;
        peakRSS /= 1024;

        auto color = [&](size_t rssValue)
        {
            if(rssValue <= 1024)
            {
                return ImVec4(0.0f, 1.0f, 0.1f, 0.3f);
            }
            if(rssValue <= 1024 * 1024)
            {
                return ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
            }
            if(rssValue <= 1024 * 1024 * 1024)
            {
                return ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
            }
            if(rssValue <= size_t(1024) * size_t(1024) * size_t(1024) * size_t(2))
            {
                return ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
            }
            if(rssValue <= size_t(1024) * size_t(1024) * size_t(1024) * size_t(24))
            {
                return ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
            }
            return ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
        };

        ImGui::SetCurrentContext(ctx);
        ImGui::Begin("Memory", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::PushStyleColor(ImGuiCol_Text, color(rssMem));
        ImGui::Text("RAM memory usage:  %" PRIu64 " KBytes", rssMem);
        ImGui::PopStyleColor();

        ImGui::PushStyleColor(ImGuiCol_Text, color(peakRSS));
        ImGui::Text("Peak RAM memory usage:  %" PRIu64 " KBytes", peakRSS);
        ImGui::PopStyleColor();
        ImGui::ProgressBar((float(rssMem) / float(totalMemory)));
        ImGui::End();
    }
}