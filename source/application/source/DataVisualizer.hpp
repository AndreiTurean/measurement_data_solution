#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include <string>
#include <Receiver.hpp>
#include "Visualizer.hpp"

namespace application
{
    class DataVisualizer:
        public NotifySubjects,
        public Visualizer,
        public MeasurementObject
    {
        std::string name_;
        bool visible_;
        bool needConfiguration_;
        uint64_t pkgHandle_;
        uint64_t pkgTimestamp_;
        size_t pkgSize_;
        ReceiverSinkManager* registeredReceiverMgr_;
        std::string processorName_;
        uint8_t instanceNb_;
        uint64_t handle_;
        MeasurementObjectType type_;
    public:
        DataVisualizer(const std::string& name, uint8_t instanceNb);
        virtual void Show(std::shared_ptr<core::ConfigurationManager>& cfgMgr);
        virtual bool notify(DataPackageCPtr pkg);
        virtual const uint8_t& getInstanceNumber();
        virtual const uint64_t& getHandle();
        virtual const MeasurementObjectType& getType();
        virtual const std::string& getName();

        friend bool operator==(const std::shared_ptr<DataVisualizer>& lhs, const std::shared_ptr<DataVisualizer>& rhs)
        {
            return lhs->getHandle() == rhs->getHandle();
        }
    };
}