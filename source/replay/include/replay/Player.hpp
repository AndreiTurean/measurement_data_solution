#pragma once
#include <defs/MdsInterface.hpp>
#include <PlayerDefs.hpp>

/*!
*   @brief Data replay
*/
namespace replay
{
    class Player : 
        public PlayerMovement,
        public PlayerInterface,
        public InterfaceAccess
    {
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
        virtual bool play();
        virtual bool pause();

        //! InterfaceAccess interface implementation
        virtual void* getInterface(const std::string& interfaceName);
    };
}