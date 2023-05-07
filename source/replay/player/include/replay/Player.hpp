#pragma once
#include <visibility.h>
#include <defs/MdsInterface.hpp>
#include <PlayerDefs.hpp>
#include <defs/Configuration.hpp>

class LoggingInterface;
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
        LoggingInterface* logger_;
        FactoryMap readerMap_;
        std::map<std::string, ReaderIfc*> activeReaderPool_;
        ReaderIfc* activeReader_;
        InterfaceAccess* interfaceAccess_;
        std::string currentLoadedRecordingFile_;
        bool showAbout_;
        
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

        bool open(const std::string& path);
        void close();
    };
}