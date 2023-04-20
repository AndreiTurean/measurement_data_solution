#pragma once
#include <visibility.h>
#include <defs/MdsInterface.hpp>
#include <PlayerDefs.hpp>

/*!
*   @brief Data replay
*/
namespace replay
{
    class REPLAY_API Player : 
        public PlayerMovement,
        public PlayerInterface,
        public InterfaceAccess,
        public GuiControlIfc
    {
        bool show_;
    public:
        /*!
        *   @brief Player constructor
        *   @param accessPtr Interface access pointer
        */
        Player(InterfaceAccess* accessPtr);
        virtual ~Player();
        Player(const Player& lhs);
        Player& operator=(const Player& lhs);

        //! PlayerMovement interface implementation
        bool move(const uint64_t steps = 1, Direction direction = Direction::forward);

        //! PlayerInterface interface implementation
        bool play();
        bool pause();

        //! InterfaceAccess interface implementation
        void* getInterface(const std::string& interfaceName);

        void show(ImGuiContext* ctx) override;
    };
}