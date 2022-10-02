#pragma once
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <stdio.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include <string>
#include <defs/Distribution.hpp>
#include "Visualizer.hpp"

namespace application
{
    class DistributionVisualizer:
        public Visualizer,
        public MeasurementObject
    {
        std::string name_;
        bool visible_;
        bool needConfiguration_;
        uint64_t pkgHandle_;
        uint64_t pkgTimestamp_;
        size_t pkgSize_;
        DataDistributionStatistics* dataDistributionStatPtr_;
        std::string processorName_;
        uint8_t instanceNb_;
        uint64_t handle_;
        MeasurementObjectType type_;
    public:
        DistributionVisualizer(const std::string& name, uint8_t instanceNb, DataDistributionStatistics* stat);
        virtual void Show(ConfigurationParser*) override;
        virtual void Reset() override;
        virtual const uint8_t& getInstanceNumber() override;
        virtual const uint64_t& getHandle() override;
        virtual const MeasurementObjectType& getType() override;
        virtual const std::string& getName() override;

        friend bool operator==(const std::shared_ptr<DistributionVisualizer>& lhs, const std::shared_ptr<DistributionVisualizer>& rhs)
        {
            return lhs->getHandle() == rhs->getHandle();
        }
    };
}