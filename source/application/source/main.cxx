// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <stdio.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include <core/Engine.hpp>
#include <imgui_stdlib.h>
#include "DataVisualizer.hpp"
#include "DistributionVisualizer.hpp"

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int, char**)
{
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Measurement data solution", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    // Our state
    //bool show_demo_window = true;
    //bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    std::shared_ptr<core::Engine> engine = std::make_shared<core::Engine>();
    engine->initialize();
    
    ConfigurationParser* configMgr = static_cast<ConfigurationParser*>(engine->getInterface("ConfigurationParser"));
    ConfigurationFactory* configFactory = static_cast<ConfigurationFactory*>(engine->getInterface("ConfigurationFactory"));
    bool showConfigMgr = false;
    uint8_t instanceNb = 0;
    bool showMetrics = true;
    bool showConfigMgrStats = false;
    std::set<VisualizerPtr> visualizerPool;

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 2. Show engine stats
        {
            if(showMetrics)
            {
                ImGui::ShowMetricsWindow(&showMetrics);
            }
            ImGui::Begin("Engine stats", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::Text("Watchdog active: %s", engine->isWatchDogActive() ? "Yes" : "No");
            ImGui::Text("Data distribution active: %s", engine->isPerformingDataAquisition() ? "Yes" : "No");
            ImGui::Text("Logging active: %s", engine->isLoggerActive() ? "Yes" : "No");
            if (ImGui::Button("Reset"))
            {
                for(auto& vis : visualizerPool)
                {
                    vis->Reset();
                }
                visualizerPool.clear();
                engine->terminate();
                engine.reset();
                engine = std::make_shared<core::Engine>();
                engine->initialize();
                configMgr = static_cast<ConfigurationParser*>(engine->getInterface("ConfigurationParser"));;
            }
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            if (ImGui::Button(showConfigMgr ? "Close configuration manager" : "Open configuration manager"))
            {
                showConfigMgr = !showConfigMgr;
            }
            if (ImGui::Button(showConfigMgrStats ? "Close configuration manager stats" : "Open configuration manager stats"))
            {
                showConfigMgrStats = !showConfigMgrStats;
            }
            if(ImGui::Button(showMetrics ? "Close debug metrics" : "Open debug metrics"))
            {
                showMetrics = !showMetrics;
            }

            ImGui::End();
        }

        for(auto& vis : visualizerPool)
        {
            vis->Show(configMgr);
        }

        if(showConfigMgrStats)
        {
            auto moList = configMgr->getMOsAddedInConfig();
            ImGui::Begin("Configuration manager stats", &showConfigMgrStats, ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::Text("Available measurement object from factory: %zu", configFactory->getFactorySize());
            ImGui::Text("Object added in the configuration manger: %zu", moList.size());
            if (ImGui::Button(showConfigMgr ? "Close configuration manager" : "Open configuration manager"))
            {
                showConfigMgr = !showConfigMgr;
            }
           
            ImGui::End();
        }

        if (showConfigMgr)
        {
            ImGui::Begin("Configuration manager", &showConfigMgr, ImGuiWindowFlags_AlwaysAutoResize);
            if(ImGui::TreeNode("Available measurement object to be created"))
            {
                for(auto object : configFactory->getFactoryMap())
                {
                    if (ImGui::Button(object.first.c_str()))
                    {
                        configMgr->createMeasurementObject(object.first, instanceNb++);
                    }
                }

                if(ImGui::Button("Raw Data Visualizer"))
                {
                    visualizerPool.insert(std::make_shared<application::DataVisualizer>("Raw Data Visualizer", instanceNb++));
                }
                if(ImGui::Button("Data Distribution Visualizer"))
                {
                    visualizerPool.insert(std::make_shared<application::DistributionVisualizer>("Data Distribution Visualizer", instanceNb++,
                        static_cast<DataDistributionStatistics*>(engine->getInterface("DataDistributionStatistics"))));
                }
            
                ImGui::TreePop();
            }
            if(ImGui::TreeNode("Created Measurement Objects"))
            {
                if(ImGui::TreeNode("Data processors"))
                {
                    for(auto object :configMgr->getMOsAddedInConfig())
                    {
                        std::string nodeName = object->getName() + std::to_string(object->getInstanceNumber());

                        if(object->getType() == MeasurementObjectType::data_receiver)
                        {
                            if(ImGui::TreeNode(nodeName.c_str()))
                            {
                                ImGui::Text("Handle: %lX", object->getHandle());
                                ImGui::Text("Instance number: %d", (int)object->getInstanceNumber());
                                ImGui::Text("Type: Receiver");
                                ImGui::TreePop();
                            }
                        }
                    }
                    ImGui::TreePop();
                }
                if(ImGui::TreeNode("Data transmitters"))
                {
                    for(auto object :configMgr->getMOsAddedInConfig())
                    {
                        std::string nodeName = object->getName() + std::to_string(object->getInstanceNumber());

                        if(object->getType() == MeasurementObjectType::data_source)
                        {
                            if(ImGui::TreeNode(nodeName.c_str()))
                            {
                                ImGui::Text("Handle: %lX", object->getHandle());
                                ImGui::Text("Instance number: %d", (int)object->getInstanceNumber());
                                ImGui::Text("Type: Transmitter");
                                ImGui::TreePop();
                            }
                        }
                    }
                    ImGui::TreePop();
                }

                if(ImGui::TreeNode("Player"))
                {
                    for(auto object :configMgr->getMOsAddedInConfig())
                    {
                        std::string nodeName = object->getName() + std::to_string(object->getInstanceNumber());

                        if(object->getType() == MeasurementObjectType::player)
                        {
                            if(ImGui::TreeNode(nodeName.c_str()))
                            {
                                ImGui::Text("Handle: %lX", object->getHandle());
                                ImGui::Text("Instance number: %d", (int)object->getInstanceNumber());
                                ImGui::Text("Type: Player");
                                ImGui::TreePop();
                            }
                        }
                    }
                    ImGui::TreePop();
                }

                if(ImGui::TreeNode("Recorder"))
                {
                    for(auto object :configMgr->getMOsAddedInConfig())
                    {
                        std::string nodeName = object->getName() + std::to_string(object->getInstanceNumber());

                        if(object->getType() == MeasurementObjectType::recorder)
                        {
                            if(ImGui::TreeNode(nodeName.c_str()))
                            {
                                ImGui::Text("Handle: %lX", object->getHandle());
                                ImGui::Text("Instance number: %d", (int)object->getInstanceNumber());
                                ImGui::Text("Type: Recorder");
                                ImGui::TreePop();
                            }
                        }
                    }
                    ImGui::TreePop();
                }

                if(ImGui::TreeNode("System"))
                {
                    for(auto object :configMgr->getMOsAddedInConfig())
                    {
                        std::string nodeName = object->getName() + std::to_string(object->getInstanceNumber());

                        if(object->getType() == MeasurementObjectType::system)
                        {
                            if(ImGui::TreeNode(nodeName.c_str()))
                            {
                                ImGui::Text("Handle: %lX", object->getHandle());
                                ImGui::Text("Instance number: %d", (int)object->getInstanceNumber());
                                ImGui::Text("Type: System");
                                ImGui::TreePop();
                            }
                        }
                    }
                    ImGui::TreePop();
                }

                if(ImGui::TreeNode("Visualization"))
                {
                    for(const auto& visMo : visualizerPool)
                    {
                        auto mo = std::dynamic_pointer_cast<MeasurementObject>(visMo);
                        if(ImGui::TreeNode(mo->getName().c_str()))
                        {
                            ImGui::Text("Handle: %lX", mo->getHandle());
                            ImGui::Text("Instance number: %d", (int)mo->getInstanceNumber());
                            ImGui::Text("Type: Visualization");
                            ImGui::TreePop();
                        }
                    }
                    ImGui::TreePop();
                }

                ImGui::TreePop();
            }
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    engine->terminate();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}