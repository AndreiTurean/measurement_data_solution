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
        public Visualizer
    {
        std::string name_;
        bool visible_;
        bool needConfiguration_;
        uint64_t pkgHandle_;
        uint64_t pkgTimestamp_;
        size_t pkgSize_;
        ReceiverSinkManager* registeredReceiverMgr_;
        std::string processorName_;
    public:
        DataVisualizer(const std::string& name);
        virtual void Show(std::shared_ptr<core::ConfigurationManager>& cfgMgr);
        virtual bool notify(DataPackageCPtr pkg);
    };
}