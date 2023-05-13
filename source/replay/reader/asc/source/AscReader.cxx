#include "pch.h"
#include <asc/AscReader.hpp>
#include <defs/DataPackage.hpp>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>

namespace replay
{
    namespace asc
    {
        AscReader::AscReader(InterfaceAccess* interfaceAccess):
            interfaceAccess_(interfaceAccess),
            extension_(".asc"),
            endPosition_(-1)
        {
            
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
            return reader_.is_open() ? (uint64_t)reader_.tellg() : -1;
        }

        uint64_t AscReader::getEndPosition()
        {
            return endPosition_;
        }

        void AscReader::setPositon(uint64_t position)
        {
            reader_.seekg(position, std::ios::cur);
        }

        DataPackagePtr AscReader::readData()
        {
            DataPackagePtr pkg = std::make_shared<DataPackage>();
            std::string line;

            while (std::getline(reader_, line))
            {
                if(line.find("SymbolicName") != std::string::npos)
                {
                    std::stringstream ss(line);
                    std::string timestamp, type, unneeded, id, size;
                    ss >> timestamp >> type >> unneeded >> unneeded >> id >> unneeded >> unneeded >> unneeded >> unneeded >> size;
                    std::remove_if(timestamp.begin(), timestamp.end(), [](char c) { return !std::isdigit(c); });
                    pkg->timestamp = std::stoull(timestamp);

                    if(type.find("CAN") != std::string::npos)
                    {
                        pkg->type = PackageType::can;
                    }

                    pkg->cycle = std::stoul(id, nullptr, 16);
                    pkg->sourceHandle = 0xDEAD;
                    pkg->size = std::stoul(size, nullptr, 10);

                    char* rawData = new char[pkg->size];
                    pkg->payload = new uint8_t[pkg->size];

                    for(uint16_t idx = 0; idx < pkg->size; ++idx)
                    {
                        std::string data;
                        ss >> data;
                        rawData[idx] = std::stoul(data, nullptr, 16);
                    }
                    std::memcpy(pkg->payload, rawData, pkg->size);
                    delete[] rawData;


                    return pkg;
                }
            }

            return nullptr;
        }

        bool AscReader::openFile(const std::string& fileName)
        {
            reader_.open(fileName, std::ios::in);

            reader_.seekg(0, std::ios::end);
            endPosition_ = reader_.tellg();
            reader_.seekg(0, std::ios::beg);

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