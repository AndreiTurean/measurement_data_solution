#pragma once
#include <visibility.h>
#include <defs/MdsInterface.hpp>
#include <defs/MdsInterface.hpp>

namespace replay
{
    namespace asc
    {
        class ASC_READER_API AscReader :
            public ReaderIfc,
            public InterfaceAccess,
            public GuiControlIfc
        {
        public:
            AscReader(InterfaceAccess* interfaceAccess);
            virtual ~AscReader();

            //! ReaderIfc interface implementation
            uint64_t getCurrentPosition() override;
            uint64_t getEndPosition() override;
            void setPositon(uint64_t position) override;
            DataPackagePtr readData(uint8_t* data, uint64_t size) override;
            bool openFile(const std::string& fileName) override;
            void closeFile() override;
            const std::string& getExtension() override;
            

            //! InterfaceAccess interface implementation
            void* getInterface(const std::string& interfaceName) override;

            //! GuiControlIfc interface implementation
            void show(ImGuiContext* ctx) override;

            
        private:
            InterfaceAccess* interfaceAccess_;
            std::fstream reader_;
            std::string extension_;
        };
    }
}

extern "C" ASC_READER_API ReaderIfc* createReader(InterfaceAccess* interfaceAccess)
{
    return new replay::asc::AscReader(interfaceAccess);
}