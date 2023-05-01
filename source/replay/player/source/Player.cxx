#include "pch.h"
#include <replay/Player.hpp>
#include <utilis/LibUtility.hpp>
#include <defs/Log.hpp>

namespace replay
{
    Player::Player(InterfaceAccess* interfaceAccess)
    {
        show_ = true;

        logger_ = static_cast<LoggingInterface*>(interfaceAccess->getInterface("LoggingInterface"));
        logger_->subscribe("Player", PLAYER_HANDLE);
        auto utilityLibrary = core::utility::LibUtility(logger_);
        logger_->log("Player was initialized, starting to load libraries", PLAYER_HANDLE, severity::information);

        for(auto& obj : std::filesystem::recursive_directory_iterator(std::filesystem::current_path()))
        {
            auto extension = obj.path().filename().extension().string();

            if(extension != ".so" && extension != ".dll")
            {   
                logger_->log(("Skipping file: " + obj.path().filename().string()).c_str(), PLAYER_HANDLE);
                continue;
            }

            void* func = utilityLibrary.openLibrary(obj.path().string(), "createReader");

            if(func)
            {
                logger_->log(("Loaded library: " + obj.path().filename().string()).c_str(), PLAYER_HANDLE, severity::information);
                readerMap_[obj.path().filename().u8string()] = func;
            }
            else
            {
                if(utilityLibrary.openLibrary(obj.path().string(), "createMO") == nullptr)
                {
                    logger_->log(("Failed to load library: " + obj.path().filename().string()).c_str(), PLAYER_HANDLE, severity::warning);
                }
            }
        }
    }

    Player::~Player()
    {

    }

    Player::Player(const Player&)
    {
        
    }

    Player& Player::operator=(const Player&)
    {
        return *this;
    }

    //! PlayerMovement interface implementation
    bool Player::move(const uint64_t, Direction)
    {
        return false;
    }

    //! PlayerInterface interface implementation
    bool Player::play()
    {
        return false;
    }

    bool Player::pause()
    {
        return false;
    }

    bool Player::open(const std::string&)
    {
        return false;
    }

    void Player::close()
    {

    }

    //! InterfaceAccess interface implementation
    void* Player::getInterface(const std::string&)
    {
        return nullptr;
    }

    void Player::show(ImGuiContext* ctx)
    {
        ImGui::SetCurrentContext(ctx);
        ImGuiViewport* viewport = (ImGuiViewport*)(void*)ImGui::GetMainViewport();
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;
        float height = ImGui::GetFrameHeight();
            
        if (ImGui::BeginViewportSideBar("##SecondaryMenuBar", viewport, ImGuiDir_Up, height, window_flags))
        {
            if (ImGui::BeginMenuBar()) {
                ImGui::ProgressBar(0.5f, ImVec2(0.f, 0.f), "Recording replayed status");
                ImGui::SameLine();
                ImGui::Button("Play forward");
                ImGui::EndMenuBar();
            }
            ImGui::End();
        }

        if (ImGui::BeginViewportSideBar("##MainStatusBar", viewport, ImGuiDir_Down, height, window_flags))
        {
            if (ImGui::BeginMenuBar()) {
                
                ImGui::EndMenuBar();
            }
            ImGui::End();
        }

        if(ImGui::BeginMainMenuBar())
        {

            if (ImGui::BeginMenu("Player"))
            {
                if (ImGui::MenuItem("Open", "Ctrl+o")) { }
                if (ImGui::MenuItem("Close", "Ctrl+w")) { }
                if (ImGui::MenuItem("Play", "Ctrl+p")) { }
                if (ImGui::MenuItem("Pause", "Ctrl+u")) {}
                if (ImGui::MenuItem("Move forward", "Ctrl+f")) { }
                if (ImGui::MenuItem("Move backward", "Ctrl+b")) { }
                if (ImGui::MenuItem("Move to start", "Ctrl+s")) {}
                if (ImGui::MenuItem("Move to end", "Ctrl+e")) {}
                if (ImGui::MenuItem("Move to", "Ctrl+m")) {}
                if (ImGui::MenuItem("Move to next", "Ctrl+n")) {}
                if (ImGui::MenuItem("Move to previous", "Ctrl+r")) {}
                if (ImGui::MenuItem("Move to first", "Ctrl+t")) {}

                ImGui::EndMenu();
            }
  
            ImGui::EndMainMenuBar();
        }
        
        
        if(ImGui::Begin("Reader factory", &show_, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::BeginTable("Reader table", 2, ImGuiTableFlags_Borders);
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Reader name");
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("Function location");

            for(const auto& [name, func] : readerMap_)
            {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%s", name.c_str());
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%p", func);
            }

            ImGui::EndTable();
            
        }
        ImGui::End();
    }

}