#include "pch.h"
#include <replay/Player.hpp>

namespace replay
{
    Player::Player(InterfaceAccess*)
    {
        show_ = true;
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

    //! InterfaceAccess interface implementation
    void* Player::getInterface(const std::string&)
    {
        return nullptr;
    }

    void Player::show(ImGuiContext* ctx)
    {
        ImGui::SetCurrentContext(ctx);
        
        if(ImGui::Begin("Player", &show_, ImGuiWindowFlags_AlwaysAutoResize))
        {
        }
        ImGui::End();
    }

}