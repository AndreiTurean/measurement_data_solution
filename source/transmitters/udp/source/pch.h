#pragma once

// PCAP PLUS PLUS library
#include <PcapLiveDeviceList.h>
#include <SystemUtils.h>
#include <Packet.h>

#include <defs/GuiDefs.hpp>

// C++ standard library
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <functional>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <algorithm>

// C standard library
#include <cstdint>
#include <cstring>
#include <inttypes.h>

// ImGui library
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <stdio.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include <imgui_stdlib.h>

#define VERSION_MAJOR 0
#define VERSION_MINOR 2

#if DEBUG
#define PROJECT_TYPE "Debug"
#else
#define PROJECT_TYPE "Release"
#endif