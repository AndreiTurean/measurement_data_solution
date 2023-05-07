#include "pch.h"
#include <replay/Player.hpp>
#include <utilis/LibUtility.hpp>
#include <defs/Log.hpp>
#include <defs/GuiDefs.hpp>
#include <filesystem>

typedef ReaderIfc* createReader(InterfaceAccess*);

namespace replay
{
    Player::Player(InterfaceAccess* interfaceAccess):
        activeReader_(nullptr),
        interfaceAccess_(interfaceAccess)
    {
        show_ = true;
        showAbout_ = false;

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

                createReader* rdr = (createReader*)func;
                ReaderIfc* reader = rdr(interfaceAccess_);
                activeReaderPool_[reader->getExtension()] = reader;
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
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_AlwaysAutoResize;
        
        BEGIN_MAIN_MENU_BAR
            BEGIN_MENU("About")
                ADD_MENU_ITEM("About player", "Ctrl+e", showAbout_)
            END_MENU
        END_MAIN_MENU_BAR

        DISPLAY_ABOUT_MENU(showAbout_, "About player")

        if (ImGui::BeginViewportSideBar("##SecondaryMenuBar", viewport, ImGuiDir_Up, TOP_BAR_HEIGHT, window_flags))
        {
            if (ImGui::BeginMenuBar())
            {
                ImGui::ProgressBar(0.0f, ImVec2(0.f, 0.f));
                ImGui::SameLine();
                ImGui::Button("Jump to begining");
                ImGui::Button("Play forward");
                ImGui::Button("Play backward");
                ImGui::Button("Jump to end");

                ImGui::EndMenuBar();
            }
            ImGui::End();
        }

        if (ImGui::BeginViewportSideBar("##MO toolbar", viewport, ImGuiDir_Right, RIGHT_BAR_WIDTH, window_flags))
        {
            if (ImGui::TreeNodeEx("Player", ImGuiTreeNodeFlags_Framed))
            {
                if(ImGui::TreeNodeEx("Player settings", ImGuiTreeNodeFlags_Framed))
                {
                    ImGui::TreePop();
                }
                if(ImGui::TreeNodeEx("Player controls", ImGuiTreeNodeFlags_Framed))
                {
                    ImGui::TreePop();
                }
                if(ImGui::TreeNodeEx("Player status", ImGuiTreeNodeFlags_Framed))
                {
                    ImGui::TreePop();
                }
                if(ImGui::TreeNodeEx("Player info", ImGuiTreeNodeFlags_Framed))
                {

                    if(ImGui::InputText("Recording name", &currentLoadedRecordingFile_, ImGuiInputTextFlags_EnterReturnsTrue))
                    {
                        logger_->log(("Changed recording name to: " + currentLoadedRecordingFile_).c_str(), PLAYER_HANDLE, severity::information);
                        std::string extension = std::filesystem::path(currentLoadedRecordingFile_).extension().string();

                        if(activeReaderPool_.find(extension) != activeReaderPool_.end())
                        {
                            activeReader_ = activeReaderPool_[extension];

                            if(activeReader_->openFile(currentLoadedRecordingFile_))
                            {
                                logger_->log(("Opened file: " + currentLoadedRecordingFile_).c_str(), PLAYER_HANDLE, severity::information);
                            }
                            else
                            {
                                logger_->log(("Failed to open file: " + currentLoadedRecordingFile_).c_str(), PLAYER_HANDLE, severity::error);
                            }
                        }
                        else
                        {
                            logger_->log(("Failed to find reader for extension: " + extension).c_str(), PLAYER_HANDLE, severity::error);
                        }
                    }
                    
                    ImGui::TreePop();
                }
                if(ImGui::TreeNodeEx("Reader factory", ImGuiTreeNodeFlags_Framed))
                {
                    if(ImGui::BeginTable("Reader table", 2, ImGuiTableFlags_Borders))
                    {
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

                    if(ImGui::BeginTable("Active reader table", 2, ImGuiTableFlags_Borders))
                    {
                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::Text("Reader extension");
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("Reader location");

                        for(const auto& [name, reader] : activeReaderPool_)
                        {
                            ImGui::TableNextRow();
                            ImGui::TableSetColumnIndex(0);
                            ImGui::Text("%s", name.c_str());
                            ImGui::TableSetColumnIndex(1);
                            ImGui::Text("%p", (void*)reader);
                        }

                        ImGui::EndTable();
                    }
                    ImGui::TreePop();
                }
                ImGui::TreePop();
            }
            ImGui::End();
        }
    }

}