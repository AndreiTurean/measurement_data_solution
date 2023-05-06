#include "pch.h"
#include <asc/AscReader.hpp>
#include <defs/DataPackage.hpp>

namespace replay
{
    namespace asc
    {
        AscReader::AscReader(InterfaceAccess* interfaceAccess)
        {
            interfaceAccess_ = interfaceAccess;
            extension_ = ".asc";
        }

        AscReader::~AscReader()
        {
            reader_.close();
        }

        void* AscReader::getInterface(const std::string&)
        {
            return nullptr;
        }

        uint64_t AscReader::getCurrentPosition()
        {
            return reader_.tellg();
        }

        uint64_t AscReader::getEndPosition()
        {
            return reader_.tellg();
        }

        void AscReader::setPositon(uint64_t position)
        {
            reader_.seekg(position);
        }

        DataPackagePtr AscReader::readData()
        {
            //reader_.read(reinterpret_cast<char*>(data), size);
            return nullptr;
        }

        bool AscReader::openFile(const std::string& fileName)
        {
            reader_.open(fileName, std::ios::in);
            return reader_.is_open();
        }

        void AscReader::closeFile()
        {
            reader_.close();
        }

        void AscReader::show(ImGuiContext* ctx)
        {
            ImGui::SetCurrentContext(ctx);
            if(ImGui::Begin("AscReader"))
            {
                ImGui::Text("Hello from AscReader");
            }
            ImGui::End();
        }

        const std::string& AscReader::getExtension()
        {
            return extension_;
        }
    }
}