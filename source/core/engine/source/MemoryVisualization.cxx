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

        rssMem /= 1024 * 1024;
        peakRSS /= 1024 * 1024;
        totalMemory /= 1024;

        auto color = [&](size_t rssValue)
        {
            if(rssValue <= 10)
            {
                return ImVec4(0.0f, 1.0f, 0.1f, 0.3f);
            }
            if(rssValue <= 1024  * 2)
            {
                return ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
            }
            if(rssValue <= 1024 * 4)
            {
                return ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
            }
            if(rssValue <= size_t(1024) * size_t(8))
            {
                return ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
            }
            if(rssValue <= totalMemory)
            {
                return ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
            }
            return ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
        };

        ImGui::SetCurrentContext(ctx);
        
        ImGui::Begin("Engine data", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::PushStyleColor(ImGuiCol_Text, color(rssMem));
        ImGui::Text("RAM memory usage:  %" PRIu64 " MBytes", rssMem);
        ImGui::PopStyleColor();

        ImGui::PushStyleColor(ImGuiCol_Text, color(peakRSS));
        ImGui::Text("Peak RAM memory usage:  %" PRIu64 " MBytes", peakRSS);
        ImGui::PopStyleColor();
        
        ImGui::PushStyleColor(ImGuiCol_Text, color(rssMem));
        ImGuiIO& io = ImGui::GetIO();
        ImGui::ProgressBar(float(float(rssMem) / float(totalMemory)), ImVec2(io.DisplaySize.x, 0.0f), ("Percentage of RAM used by the application: " + std::to_string(float(float(rssMem) / float(totalMemory)) * 100.0f) + "%").c_str());
        ImGui::PopStyleColor();
        
        ImGui::SetWindowPos(ImVec2(0, io.DisplaySize[1] - ImGui::GetWindowSize().y));
        ImGui::End();
    }
}